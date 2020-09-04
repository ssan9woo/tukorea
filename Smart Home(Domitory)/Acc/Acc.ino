#include <ESP8266WiFi.h>
#include <IBMIOTDevice_Gatway.h>
#include <Wire.h>
#define Addr 0x1C

String user_html = ""
    "<p><input type='text' name='edge' placeholder='Edge Address'>"
    "<p><input type='text' name='devType' placeholder='Device Type'>"
    "<p><input type='text' name='devId' placeholder='Device Id'>"
    "<p><input type='text' name='meta.pubInterval' placeholder='Publish Interval'>";

char*               ssid_pfix = (char*)"sang";
unsigned long       pubInterval;
unsigned long       lastPublishMillis = 0;


//디바이스 만들고 -> 캡티브 포탈로 잡아주고 -> pubinterval로 publish 하고 그 보드에서 나오는거 확인

void publishData() {
    unsigned int adata[7];
    unsigned int bdata[7];
    int Accla[3] = {0,};
    int Acclb[3] = {0,};
    char accToCharArr[30];
    int result = 0;
    
    Wire.requestFrom(Addr, 7);
    if(Wire.available() == 7){ // read received data
      for(int i = 0; i < 7; i++) adata[i] = Wire.read();
    }
    for(int i = 0; i < 3; i ++){
      Accla[i] = ((adata[i*2 + 1] * 256) + adata[i*2 + 2]) / 16;
      if(Accla[i] > 2047) Accla[i] -= 4096;
    }

    Wire.requestFrom(Addr, 7);
    if(Wire.available() == 7){ // read received data
      for(int i = 0; i < 7; i ++) adata[i] = Wire.read();
    }
    for(int i = 0; i < 3; i ++){
      Acclb[i] = ((adata[i*2 + 1] * 256) + bdata[i*2 + 2]) / 16;
      if(Acclb[i] > 2047) Acclb[i] -= 4096;
    }
    
    for(int i = 0; i < 3; i++){
      result += (Accla[i] - Acclb[i]);
    }
    
    sprintf(accToCharArr,"%d",result);
    
  
    StaticJsonBuffer<512> jsonOutBuffer;
    JsonObject& root = jsonOutBuffer.createObject();
    JsonObject& data = root.createNestedObject("d");

    data["acc"] = accToCharArr;
    root.printTo(msgBuffer, sizeof(msgBuffer));
    client.publish(publishTopic, msgBuffer);
}

void handleUserCommand(JsonObject* root) {
    JsonObject& d = (*root)["d"];
    // user command

    lastPublishMillis = 0;
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
    Wire.begin();
    Serial.begin(115200);

    //Wire setting------------------------------------
    Wire.beginTransmission(Addr);
    Wire.write(0x2A);
    Wire.write((byte)0x00);
    Wire.endTransmission();

    Wire.beginTransmission(Addr);
    Wire.write(0x2A);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.beginTransmission(Addr);
    Wire.write(0x0E);
    Wire.write((byte)0x00);
    Wire.endTransmission();
    //------------------------------------------------

    
    initCheck();
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
    client.setServer(iot_server, 1883);   
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
