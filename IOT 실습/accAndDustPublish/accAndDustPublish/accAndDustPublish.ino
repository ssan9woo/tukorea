
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#define Addr 0x1C


// 캡티브 포탈로 바꾸기
char ssid[] = "IoT518"; //"KPU_WiFi858"; //"IoT518";
char password[] = "iot123456";
const char* mqttServer = "54.80.90.212"; //"3.92.133.55"; // AWS에서 부여받은 개인의 ip 주소 // 18.206.251.222
const int mqttPort = 1883; // Port 할당
const char* mqttUser = "yhur1";
const char* mqttPassword = "hi1";
const char* mqtt_topic = "esp/rebi/evt/Accelration";



String str_acc;
char Msg[5];
int Accelration;
int i;
HTTPClient http;
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(115200);
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x2A);
  // StandBy mode
  Wire.write((byte)0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x2A);
  // Active mode
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();
 
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x0E);
  // Set range to +/- 2g
  Wire.write((byte)0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  delay(300);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println("");
  
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) 
    {
        Serial.println("Connecting to MQTT...");
        if (client.connect("ESP8266Client1", mqttUser, mqttPassword )) 
        {
            Serial.println("connected");
        } 
        else 
        {
            Serial.print("failed with state "); Serial.println(client.state());
            delay(2000);
        }
    }
}

void loop()
{
  unsigned int adata[7];
  unsigned int bdata[7];
  // Request 7 bytes of data
  Wire.requestFrom(Addr, 7);
  // Read 7 bytes of data
  // staus, xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
  if(Wire.available() == 7) // read received data
  {for(i = 0; i<7; i++) adata[i] = Wire.read();}
  // Convert the data to 12-bits
  int xAccla = ((adata[1] * 256) + adata[2]) / 16;
  if (xAccla > 2047) xAccla -= 4096;
  int yAccla = ((adata[3] * 256) + adata[4]) / 16;
  if (yAccla > 2047) yAccla -= 4096;
  int zAccla = ((adata[5] * 256) + adata[6]) / 16;
  if (zAccla > 2047) zAccla -= 4096;

  Wire.requestFrom(Addr, 7);
  if(Wire.available() == 7) // read received data
  {for(i = 0; i<7; i++) bdata[i] = Wire.read();}
  // Convert the data to 12-bits
  int xAcclb = ((bdata[1] * 256) + bdata[2]) / 16;
  if (xAcclb > 2047) xAcclb -= 4096;
  int yAcclb = ((bdata[3] * 256) + bdata[4]) / 16;
  if (yAcclb > 2047) yAcclb -= 4096;
  int zAcclb = ((bdata[5] * 256) + bdata[6]) / 16;
  if (zAcclb > 2047) zAcclb -= 4096;
  
  int xAccl = xAccla - xAcclb;
  int yAccl = yAccla - yAcclb;
  int zAccl = zAccla - zAcclb;
 
  // Output data to serial monitor
  Serial.print("Acceleration in X-Axis : "); Serial.println(xAccl);
  Serial.print("Acceleration in Y-Axis : "); Serial.println(yAccl);
  Serial.print("Acceleration in Z-Axis : "); Serial.println(zAccl);
  delay(100);

  client.loop();
  Accelration = xAccl + yAccl + zAccl;

  str_acc = String(Accelration);
  str_acc.toCharArray(Msg,5);
  client.publish(mqtt_topic,Msg);
}
