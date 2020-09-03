#include <ESP8266WiFi.h>
#include <IBMIOTDevice_Gatway.h>
#include <Wire.h>
#include <OLED32.h>
#include <DHT.h>

String user_html = ""
    "<p><input type='text' name='edge' placeholder='Edge Address'>"
    "<p><input type='text' name='devType' placeholder='Device Type'>"
    "<p><input type='text' name='devId' placeholder='Device Id'>"
    "<p><input type='text' name='meta.pubInterval' placeholder='Publish Interval'>";

char*               ssid_pfix = (char*)"sang";
unsigned long       pubInterval;
unsigned long       lastPublishMillis = 0;
unsigned long lastClicked = 0;

int clicked = 0;
const int pulseA = 12;
const int pulseB = 13;
const int pushSW = 2;
volatile int lastEncoded = 0;
volatile long encoderValue = 70;

OLED display(4, 5);
#define DHTTYPE DHT22
// DHT22 is attached to GPIO14 on this Kit
#define DHTPIN 14
DHT dht(DHTPIN, DHTTYPE, 11);

float humidity;
float temp_f;
unsigned long lastDHTReadMillis = 0;
#define interval 2000
float tgtT;
void handleRotary() {
    // Never put any long instruction
    int MSB = digitalRead(pulseA); //MSB = most significant bit
    int LSB = digitalRead(pulseB); //LSB = least significant bit
    int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
    int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
    if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
    if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
    lastEncoded = encoded; //store this value for next time
    if (encoderValue > 255) {
        encoderValue = 255;
    }
    else if (encoderValue < 0 ){
        encoderValue = 0;
    }
    lastPublishMillis = millis() - pubInterval + 200; // send and publish immediately
}


void publishData() {
    StaticJsonBuffer<512> jsonOutBuffer;
    JsonObject& root = jsonOutBuffer.createObject();
    JsonObject& data = root.createNestedObject("d");
    // data["temperature"]  = yourData;
    if (clicked == 1) {
        data["button"] = "clicked";
        clicked = 0;
    }
    gettemperature();
    char dht_buffer[10];
    char dht_buffer2[10];
    sprintf(dht_buffer, "%2.1f", temp_f);
    display.print(dht_buffer, 2, 10);
    data["temperature"] = dht_buffer;
    tgtT = map(encoderValue, 0, 255, 10, 50);
    sprintf(dht_buffer2, "%2.1f", tgtT);
    data["target"] = dht_buffer2;
    display.print(dht_buffer2, 3, 10);
    
    root.printTo(msgBuffer, sizeof(msgBuffer));
    client.publish(publishTopic, msgBuffer);
    Serial.println("pub");
}

void handleUserCommand(JsonObject* root) {
    JsonObject& d = (*root)["d"];
    // user command
    Serial.println("handle");
    if(d.containsKey("target")) {
        tgtT = atoi(d["target"]);
        encoderValue = map(tgtT, 10, 50, 0, 255);
        lastPublishMillis = 0;
    }
}

void buttonClicked() {
    if(millis() - lastClicked >= 300) {
        lastClicked = millis();
        clicked = 1;
        lastPublishMillis = 0;
    }
}

void message(char* topic, byte* payload, unsigned int payloadLength) {
    Serial.println("message1");
    StaticJsonBuffer<512> jsonInBuffer;
    byte2buff(msgBuffer, payload, payloadLength);
    JsonObject& root = jsonInBuffer.parseObject((char*)msgBuffer);
    if (!root.success()) {
        Serial.println("handleCommand: payload parse FAILED");
        return;
    }
    Serial.println("message2");
    handleIOTCommand(topic, &root);
    if (!strcmp(updateTopic, topic)) {
        pubInterval = (*cfg)["meta"]["pubInterval"];
    } else if (!strncmp(commandTopic, topic, 10)){            // strcmp return 0 if both string matches
        handleUserCommand(&root);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(pushSW, INPUT_PULLUP);
    pinMode(pulseA, INPUT_PULLUP);
    pinMode(pulseB, INPUT_PULLUP);
    attachInterrupt(pushSW, buttonClicked, FALLING);
    attachInterrupt(pulseA, handleRotary, CHANGE);
    attachInterrupt(pulseB, handleRotary, CHANGE);
    display.begin();
    display.print("IOT Thermostat");
    display.print("Currnt: ", 2, 2);
    display.print("Target: ", 3, 2);
    initCheck();
    // *** If no "config" is found or "config" is not "done", run configDevice ***
    if(!cfg->containsKey("config") || strcmp((const char*)(*cfg)["config"], "done")) {
         configDevice();
    }
    WiFi.mode(WIFI_STA);
    WiFi.begin((const char*)(*cfg)["ssid"], (const char*)(*cfg)["w_pw"]);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // main setup
    Serial.printf("\nIP address : "); Serial.println(WiFi.localIP());
    JsonObject& meta = (*cfg)["meta"];
    pubInterval = meta.containsKey("pubInterval") ? atoi((const char*)meta["pubInterval"]) : 0;

    sprintf(iot_server, "%s", (const char*)(*cfg)["edge"]);
    if (!espClient.connect(iot_server, 1883)) {
        Serial.println("connection failed");
        return;
    }
    client.setServer(iot_server, 1883);   //IOT
    client.setCallback(message);
    Serial.println("message3");
    iot_connect();
    Serial.println("message4");
}

void loop() {
    if (!client.connected()) {
        iot_connect();
    }
    client.loop();
    if ((pubInterval != 0) && (millis() - lastPublishMillis > pubInterval)) {
        publishData();
        lastPublishMillis = millis();
    }
}

void gettemperature() {
    unsigned long currentMillis = millis();
    if(currentMillis - lastDHTReadMillis >= interval) {
        lastDHTReadMillis = currentMillis;
        humidity = dht.readHumidity(); // Read humidity (percent)
        temp_f = dht.readTemperature(); // Read temperature as Fahrenheit
        // Check if any reads failed and exit early (to try again).
        if (isnan(humidity) || isnan(temp_f)) {
            return;
        }
    }
}
