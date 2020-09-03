#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define             EEPROM_LENGTH 512
ESP8266WebServer    webServer(80);
StaticJsonBuffer<EEPROM_LENGTH> jsonConfigBuffer;
JsonObject*         cfg;
char                cfgBuffer[EEPROM_LENGTH];
const int           RESET_PIN = 0;

extern String user_html;
extern char* ssid_pfix;

String html_begin = ""
    "<html><head><title>IOT Device Setup</title></head>"
    "<body><center><h1>Device Setup Page</h1>"
        "<style>"
            "input {font-size:3em; width:90%; text-align:center;}"
            "button { border:0;border-radius:0.3rem;background-color:#1fa3ec;"
            "color:#fff; line-height:2em;font-size:3em;width:90%;}"
        "</style>"
        "<form action='/save'>"
            "<p><input type='text' name='ssid' placeholder='SSID'>"
            "<p><input type='text' name='w_pw'placeholder='Password'>";

String html_end = ""
            "<p><button type='submit'>Save</button>"
        "</form>"
    "</center></body></html>";


void byte2buff(char* msg, byte* payload, unsigned int len) {
    unsigned int i, j;
    for (i=j=0; i < len ;) {
        msg[j++] = payload[i++];
    }
    msg[j] = '\0';
}

void reset_config() {
    int i, len;
    const char* empty="{meta:{}}";
    len=strlen(empty);
    for(i = 0 ;i < len;i++) {
        EEPROM.write(i, empty[i]);
    }
    EEPROM.write(i, '\0');
    EEPROM.commit();
}

void maskConfig(char* buff) {
    StaticJsonBuffer<EEPROM_LENGTH> jsonTempBuffer;
    char msgBuffer[EEPROM_LENGTH];

    yield();
    cfg->printTo(msgBuffer, sizeof(msgBuffer));

    JsonObject& temp_cfg = jsonTempBuffer.parseObject(String(msgBuffer));
    temp_cfg["w_pw"] = "********";
    temp_cfg["token"] = "********";
    temp_cfg.printTo(buff, EEPROM_LENGTH);
}

void init_config_json() {
    int i;
    for ( i = 0 ; EEPROM.read(i) != '\0'; i++ ) {
        cfgBuffer[i] = EEPROM.read(i);
    }
    cfgBuffer[i] = '\0';

    cfg = &(jsonConfigBuffer.parseObject(String(cfgBuffer)));
    if (cfg->success() ) {
        Serial.println("CONFIG JSON Successfully loaded");
        char maskBuffer[EEPROM_LENGTH];
        maskConfig(maskBuffer);
        Serial.println(String(maskBuffer));
    } else {
        cfg = &(jsonConfigBuffer.parseObject("{meta:{}}"));
        Serial.println("Initializing CONFIG JSON");
    }
}

void initCheck() {
    // check Factory Reset Request and reset if requested
    // and initialize
    EEPROM.begin(EEPROM_LENGTH);            // it should be before reset_config and init_config_json
    pinMode(RESET_PIN, INPUT_PULLUP);
    if( digitalRead(RESET_PIN) == 0 ) {
        unsigned long t1 = millis();
        while(digitalRead(RESET_PIN) == 0) {
            delay(500);
            Serial.print(".");
        }
        if (millis() - t1 > 5000) {
            reset_config();             // Factory Reset
        }
    }
    attachInterrupt(RESET_PIN, []() { ESP.restart(); }, FALLING);
    init_config_json();
}

void save_config_json(){
    int i, len;
    cfg->printTo(cfgBuffer, sizeof(cfgBuffer));
    len=strlen(cfgBuffer);
    for(i = 0 ;i < len;i++) {
        EEPROM.write(i, cfgBuffer[i]);
    }
    EEPROM.write(i, '\0');
    EEPROM.commit();
}

void saveEnv() {
    int args = webServer.args();
    for (int i = 0; i < args ; i++){
        if (webServer.argName(i).indexOf(String("meta.")) == 0 ) {
            (*cfg)["meta"][webServer.argName(i).substring(5)] = webServer.arg(i);
        } else {
            (*cfg)[webServer.argName(i)] = webServer.arg(i);
        }
    }
    (*cfg)["config"] = "done";
    save_config_json();
    webServer.send(200, "text/html", "rebooting the device.");
    ESP.restart();
}

void configDevice() {
    DNSServer   dnsServer;
    const byte  DNS_PORT = 53;
    IPAddress   apIP(192, 168, 1, 1);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    char ap_name[100];
    sprintf(ap_name, "%s_%08X", ssid_pfix, ESP.getChipId());
    WiFi.softAP(ap_name);
    dnsServer.start(DNS_PORT, "*", apIP);

    webServer.on("/save", saveEnv);
    webServer.onNotFound([]() {
        webServer.send(200, "text/html", html_begin + user_html + html_end);
    });
    webServer.begin();
    Serial.println("starting the config");
    while(1) {
        yield();
        dnsServer.processNextRequest();
        webServer.handleClient();
    }
}
