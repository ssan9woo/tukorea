#include <ESP8266WiFi.h>
#include <IBMIOTDevice.h>
#include <Wire.h>
#include <OLED32.h>
#include <DHT.h>


String user_html = ""
    "<p><input type='text' name='org' placeholder='org'>"
    "<p><input type='text' name='devType' placeholder='Device Type'>"
    "<p><input type='text' name='devId' placeholder='Device Id'>"
    "<p><input type='text' name='token' placeholder='Device Token'>"
    "<p><input type='text' name='meta.pubInterval' placeholder='Publish Interval'>";

char*               ssid_pfix = (char*)"suk";
unsigned long       pubInterval;
unsigned long       lastPublishMillis = 0;



OLED display(4,5);

#define DHTTYPE DHT22
#define DHTPIN 14
DHT dht(DHTPIN, DHTTYPE, 11);

float humidity;
float temp_f;
unsigned long lastDHTReadMillis = 0;
#define interval 2000


void publishData() {
    StaticJsonBuffer<512> jsonOutBuffer;
    JsonObject& root = jsonOutBuffer.createObject();
    JsonObject& data = root.createNestedObject("d");

    
    gettemperature();
    char dht_buffer[10];
    char dht_buffer2[10];

    sprintf(dht_buffer, "%2.1f", temp_f);
    display.print(dht_buffer,2, 10);
    
    data["temperature"] = dht_buffer;
    sprintf(dht_buffer2, "%2.1f", humidity);
    data["humidity"] = dht_buffer2;
    display.print(dht_buffer2,3, 10);
    
    root.printTo(msgBuffer, sizeof(msgBuffer));
    client.publish(publishTopic, msgBuffer);
}

void handleUserCommand(JsonObject* root) {
    JsonObject& d = (*root)["d"];

}

void message(char* topic, byte* payload, unsigned int payloadLength) { // 콜백함수
    StaticJsonBuffer<512> jsonInBuffer;
    byte2buff(msgBuffer, payload, payloadLength);
    JsonObject& root = jsonInBuffer.parseObject((char*)msgBuffer);
    if (!root.success()) {
        Serial.println("handleCommand: payload parse FAILED");
        return;
    }

    handleIOTCommand(topic, &root);
    if (!strcmp(updateTopic, topic)) {
        pubInterval = (*cfg)["meta"]["pubInterval"];  //eeprom에 비슷한 코드 집어넣
    } else if (!strncmp("iot-2/cmd/", topic, 10)) {            // strcmp return 0 if both string matches
        handleUserCommand(&root);
    }
}


void setup() {
    Serial.begin(115200);
    display.begin();
    display.print("IOT Thermometer");
    display.print("Temp: ", 2, 2);
    display.print("Humi: ", 3, 2);
    
    initCheck();   //초기에 eeprom에 data있는지 유무에따라 달라
    // *** If no "config" is found or "config" is not "done", run configDevice ***
    if(!cfg->containsKey("config") || strcmp((const char*)(*cfg)["config"], "done")) {
         configDevice(); //done이 있으면 configuration 완료상태.
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

    sprintf(iot_server, "%s.messaging.internetofthings.ibmcloud.com", (const char*)(*cfg)["org"]);
    if (espClient.connect(iot_server, 8883)) {
        if (!espClient.verify(fingerprint, iot_server)) {
            Serial.println("certificate doesn't match");
            return;
        }
    } else {
        Serial.println("connection failed");
        return;
    }
    client.setServer(iot_server, 8883);   //IOT
    client.setCallback(message);
    iot_connect();
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

void gettemperature()
{
  unsigned long currentMillis = millis();

  if(currentMillis - lastDHTReadMillis >= interval)
  {
    lastDHTReadMillis = currentMillis;

    humidity = dht.readHumidity();
    temp_f = dht.readTemperature();

    if(isnan(humidity) || isnan(temp_f))
    {
      return;
    }
  }
}
