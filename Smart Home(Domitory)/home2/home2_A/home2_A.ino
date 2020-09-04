
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <IBMIOTDevice2Gateway.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Servo.h>
#include <Wire.h>
#include "myButton.h"
#include "bitmap.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String user_html = ""
                   "<p><input type='text' name='edge' placeholder='Edge Address'>"
                   "<p><input type='text' name='devType' placeholder='Device Type'>"
                   "<p><input type='text' name='devId' placeholder='Device Id'>"
                   "<p><input type='text' name='meta.pubInterval' placeholder='Publish Interval'>";

char *ssid_pfix = (char *)"Cheese";
unsigned long pubInterval;
unsigned long lastPublishMillis = 0;
unsigned long lastClicked = 0;


#define DHTTYPE DHT22
// DHT22 is attached to GPIO14 on this Kit
#define DHTPIN 14
DHT dht(DHTPIN, DHTTYPE, 11);
myButton button[3];
int pins[3] = {2, 12, 13};
std::vector<int> passwd = {0, 1, 0, 0};
std::vector<int> input;

Servo door;

float humidity;
float temp_f;
unsigned long lastDHTReadMillis = 0;
#define interval 2000
int channelCount = 0;

#define OPEN_TIME 5000
enum{DCLOSED, DOPENED};
int deg[2] = {45, 135};



void ChannelChange(int count){
  //count에 따라서 배열 원소 display
  Serial.print("channel : ");
  Serial.println(count);
  display.clearDisplay();
  delay(10);
  display.drawBitmap(0, 0, screen[count], 128, 32, 1);
  display.display();
  delay(10);
}


void TvOff(){
  //tv off 초기화면으로
  display.clearDisplay();
  display.drawBitmap(0, 0, tv, 128, 32, 1);
  display.display();
  Serial.println("tv off");
}


void Tv(const char* msg){
  Serial.println(strlen(msg));
  
  if(!strcmp(msg,"up")){
        Serial.println("up");
        channelCount += 1;
        if(channelCount >= 4) channelCount = 0;
        ChannelChange(channelCount);
  }
  else if(!strcmp(msg,"down")){
        Serial.println("down");
        channelCount -= 1;
        if(channelCount < 0) channelCount = 3;
        ChannelChange(channelCount);
  }
  else if(!strcmp(msg, "off")){
        TvOff();
  }
}

void Buzzer(const char* msg){
  if(!strcmp(msg,"on")){
      OpenTheDoor();
      delay(5000);
      CloseTheDoor();
  }
}

void setupButtons()
{
    for(int i = 0; i < 3; ++i)
    {
        button[i].attach(pins[i]);
        button[i].set_debounce(90);
        button[i].init();
    }
}

int get_button()
{
    int ind = -1;
    for(int i = 0; i < 3; ++i)
    {
        if(button[i].get())
        {
            ind = i;
            break;
        }
    }

    return ind;
}

void gettemperature()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastDHTReadMillis >= interval)
    {
        lastDHTReadMillis = currentMillis;
        humidity = dht.readHumidity();  // Read humidity (percent)
        temp_f = dht.readTemperature(); // Read temperature as Fahrenheit
        // Check if any reads failed and exit early (to try again).
        if (isnan(humidity) || isnan(temp_f))
        {
            return;
        }
    }
}

void publishData()
{
    char dht_buffer[10];
    StaticJsonBuffer<512> jsonOutBuffer;
    JsonObject &root = jsonOutBuffer.createObject();
    JsonObject &data = root.createNestedObject("d");
    // data["temperature"]  = yourData;
    
    gettemperature();
    sprintf(dht_buffer, "%2.1f", temp_f);
    data["temperature"] = dht_buffer;

    if(strcmp(dht_buffer,"nan"))
    { 
      Serial.println("Published");   
      root.printTo(msgBuffer, sizeof(msgBuffer));
      client.publish(publishTopic, msgBuffer);
    }
}

void handleUserCommand(JsonObject *root)
{
    JsonObject &d = (*root)["d"];
    // user command
        char buff[50];
    d.prettyPrintTo(buff);
    Serial.println(buff);
    // user command

    if(d.containsKey("tv")){
        Tv(d["tv"]);
        Serial.println("b");
    }
    if(d.containsKey("buzzer")){
        Buzzer(d["buzzer"]);
        Serial.println("c");
    }

}



void message(char *topic, byte *payload, unsigned int payloadLength)
{
    Serial.println("message1");
    StaticJsonBuffer<512> jsonInBuffer;
    byte2buff(msgBuffer, payload, payloadLength);
    JsonObject &root = jsonInBuffer.parseObject((char *)msgBuffer);
    if (!root.success())
    {
        Serial.println("handleCommand: payload parse FAILED");
        return;
    }
    Serial.println("message2");
    handleIOTCommand(topic, &root);
    if (!strcmp(updateTopic, topic))
    {
        pubInterval = (*cfg)["meta"]["pubInterval"];
    }
    else if (!strncmp(commandTopic, topic, 10))
    { // strcmp return 0 if both string matches
        handleUserCommand(&root);
    }
}

void setup()
{
    Serial.begin(115200);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
    }
    setupButtons();
    door.attach(15);
    

    initCheck();
    // *** If no "config" is found or "config" is not "done", run configDevice ***
    if (!cfg->containsKey("config") || strcmp((const char *)(*cfg)["config"], "done"))
    {
        configDevice();
    }
    WiFi.mode(WIFI_STA);
    WiFi.begin((const char *)(*cfg)["ssid"], (const char *)(*cfg)["w_pw"]);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // main setup
    Serial.printf("\nIP address : ");
    Serial.println(WiFi.localIP());
    JsonObject &meta = (*cfg)["meta"];
    pubInterval = meta.containsKey("pubInterval") ? atoi((const char *)meta["pubInterval"]) : 0;

    sprintf(iot_server, "%s", (const char *)(*cfg)["edge"]);
    if (!espClient.connect(iot_server, 1883))
    {
        Serial.println("connection failed");
        return;
    }
    client.setServer(iot_server, 1883); //IOT
    client.setCallback(message);
    iot_connect();
    door.write(deg[DCLOSED]);
    display.clearDisplay();
    display.drawBitmap(0, 0, tv, 128, 32, 1);
    display.display();
   
}

void loop()
{
    static time_t before = 0;
    static int door_state = 0;
    if (!client.connected())
    {
        iot_connect();
    }
    client.loop();
    int ind = get_button();
    //Serial.printf("ind: %d\n", ind);
    if(ind > -1)
    {
        if(ind < 2)
        {
            input.push_back(ind);
        }
        else if(ind == 2)
        {
            Serial.print("passwd: ");
            for(int i = 0; i < passwd.size(); ++i)
            {
                Serial.print(passwd.at(i));
            }
            Serial.println("");
            Serial.print("input: ");
            for(int i = 0; i < input.size(); ++i)
            {
                Serial.print(input.at(i));
            }
            Serial.println("");
            
            bool is_correct = input == passwd;
            if(is_correct && door_state == DCLOSED)
            {
                Serial.println("door open");
                door_state = DOPENED;
                door.write(deg[DOPENED]);
                before = millis();
            }
            input.clear();
        }
    }
    if((millis() - before > OPEN_TIME) && door_state == DOPENED)
    {
        Serial.println("door close");
        door_state = DCLOSED;
        door.write(deg[DCLOSED]);
        before = millis();
    }
    if ((pubInterval != 0) && (millis() - lastPublishMillis > pubInterval))
    {
        publishData();
        lastPublishMillis = millis();
    }
}
