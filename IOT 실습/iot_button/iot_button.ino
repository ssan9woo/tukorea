#include <ESP8266WiFi.h>
#include <IBMIOTDevice.h>


String user_html = ""
    "<p><input type='text' name='org' placeholder='org'>"
    "<p><input type='text' name='devType' placeholder='Device Type'>"
    "<p><input type='text' name='devId' placeholder='Device Id'>"
    "<p><input type='text' name='token' placeholder='Device Token'>"
    "<p><input type='text' name='meta.pubInterval' placeholder='Publish Interval'>";

char*               ssid_pfix = (char*)"suk";
unsigned long       pubInterval;
unsigned long       lastPublishMillis = 0;
unsigned long       lastClicked = 0;
int clicked = 0;



void publishData() {
    StaticJsonBuffer<512> jsonOutBuffer;
    JsonObject& root = jsonOutBuffer.createObject();
    JsonObject& data = root.createNestedObject("d");

    if(clicked == 1)
    { 
      data["button"] = "clicked"; 
      clicked = 0;
    }
    
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
void buttonClicked()
{
  if(millis() - lastClicked >= 300)
  {
    lastClicked = millis();
    clicked = 1;
    lastPublishMillis = 0; // 메인루프에서 계속 인터벌 5000 3000 준거 //그 주기로 돌아감 스위치는 눌렸을때신호가가게끔하는거
  } 
}


void setup() {
    Serial.begin(115200);
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(2,buttonClicked, FALLING);
    
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
