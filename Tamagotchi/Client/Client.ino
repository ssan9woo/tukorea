#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Sangwoo"; // 와이파이 이름
const char* password = "56962353"; // 와이파이 비밀번호
#define mqtt_server "tailor.cloudmqtt.com" // MQTT server 주소
#define mqtt_port 16420 // port 번호
#define mqtt_topic "SSW" // topic (자유롭게 작성}
#define mqtt_user "tgiqgumo" // username
#define mqtt_password "vtJiGGDTNy8J" // password

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup()
{  
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2,HIGH);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop()
{
  if(!client.connected())
  {
    client.connect("ESP8266Client2", mqtt_user, mqtt_password);
    client.subscribe(mqtt_topic);
  }
  client.loop();
}
void callback(char* topic, byte* payload, unsigned int length)
{
  String Msg = "";
  int i=0;
  while (i<length) Msg += (char)payload[i++];

  if(Msg == "Hungry!!!")
  {
    for(int i = 0; i < 5; i ++)
    {
      digitalWrite(2,LOW);
      delay(300);
      digitalWrite(2,HIGH);
      delay(300); 
    }
  }
}
