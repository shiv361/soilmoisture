#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
 
#define DHTPIN D4         //pin where the dht11 is connected
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
 
String apiKey = "88317ZL6AXI2BU75"; // Enter your Write API key from ThingSpeak
const char *ssid = "Power";     // replace with your wifi ssid and wpa2 key
const char *pass = "90909090";
const char* server = "api.thingspeak.com";
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
const int AirValue = 790;   //you need to replace this value with Value_1
const int WaterValue = 390;  //you need to replace this value with Value_2
const int SensorPin = A0;
int soilMoistureValue = 0;
int soilmoisturepercent=0;
int relaypin = D5;
 int sensorValue;  
 int limit = 35; 
WiFiClient client;
 
 
void setup() {
  Serial.begin(115200); // open serial port, set the baud rate to 9600 bps
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.display(); //Display logo
  display.clearDisplay();
  pinMode(relaypin, OUTPUT);
   pinMode(13, OUTPUT);
 
  dht.begin();
  
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    delay(2000);
}  
 
 
void loop() 
{
    delay(1000);
   float moisture_percentage;
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

 sensorValue = analogRead(SensorPin); 
// Serial.println("Analog Value : ");
// Serial.println(sensorValue);
 moisture_percentage = ( 100 - ( (sensorValue/1023.00) * 100 ) );
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%\n\n");
 
 if (moisture_percentage<limit) {
 digitalWrite(D5, HIGH); 
 }
 else {
 digitalWrite(D5, LOW); 
 }




 display.display(); 

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  //
//  Serial.print(F("F  Heat index: "));
//  Serial.print(hic);
//  Serial.print(F("C "));
//  Serial.print(hif);
//  Serial.println(F("F"));
  
  soilMoistureValue = analogRead(SensorPin);  //put Sensor insert into soil
  Serial.println(soilMoistureValue);
  
  //soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
 
 
//if(soilmoisturepercent > 100)
//{
//  Serial.println("100 %");
  
  display.setCursor(0,0);  //oled display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  if(moisture_percentage<0){
  display.print("Moist:");
  display.setTextSize(2);
 display.print(0);
 display.println("%");
  }
  else{
     display.print("Moist:");
  display.setTextSize(2);
 display.print(moisture_percentage);
 display.println("%");
  }
  display.setCursor(0,20);  //oled display
  display.setTextSize(1);
  display.print("Humidity:");
  display.setTextSize(2);
  display.print(h);
  display.println("%");
  display.setCursor(0,40);  //oled display
  display.setTextSize(1);
  display.print("Temp:");
  display.setTextSize(2);
  display.print(t);
  display.println("C");
  display.display();
  

  display.clearDisplay();
//}
 

//else if(soilmoisturepercent <0)
//{
//  Serial.println("0 %");
//  
//  display.setCursor(0,0);  //oled display
//  display.setTextSize(2);
//  display.setTextColor(WHITE);
//  display.print("Soil :");
//  display.setTextSize(1);
//  display.print("0");
//  display.println(" %");
//  display.setCursor(0,20);  //oled display
//  display.setTextSize(2);
//  display.print("Air RH:");
//  display.setTextSize(2);
//  display.print(h);
//  display.println(" %");
//  display.setCursor(0,40);  //oled display
//  display.setTextSize(2);
//  display.print("Temp:");
//  display.setTextSize(2);
//  display.print(t);
//  display.println(" C");
//  display.display();
// 
//  delay(250);
//  display.clearDisplay();
//}
// 
 
//else if(soilmoisturepercent >=0 && soilmoisturepercent <= 100)
//{
//  Serial.print(soilmoisturepercent);
//  Serial.println("%");
//  
//  display.setCursor(0,0);  //oled display
//  display.setTextSize(2);
//  display.setTextColor(WHITE);
//  display.print("Soil RH:");
//  display.setTextSize(1);
//  display.print(soilmoisturepercent);
//  display.println(" %");
//  display.setCursor(0,20);  //oled display
//  display.setTextSize(1);
//  display.print("humidity:");
//  display.setTextSize(1);
//  display.print(h);
//  display.println(" %");
//  display.setCursor(0,40);  //oled display
//  display.setTextSize(2);
//  display.print("Temp:");
//  display.setTextSize(1);
//  display.print(t);
//  display.println(" C");
//  display.display();
// 
//  delay(250);
//  display.clearDisplay();
//}
 
//  if(soilmoisturepercent >=0 && soilmoisturepercent <= 30)
//  {
//    digitalWrite(relaypin, HIGH);
//    Serial.println("Motor is ON");
//  }
//  else if (soilmoisturepercent >30 && soilmoisturepercent <= 100)
//  {
//    digitalWrite(relaypin, LOW);
//    Serial.println("Motor is OFF");
//  }
  
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
//      postStr += "&field1=";
//      postStr += String(soilmoisturepercent);
      postStr += "&field1=";
      postStr += String(t);
      postStr += "&field2=";
      postStr += String(h);
      postStr += "&field3=";
      postStr += String(moisture_percentage);
//      postStr += "&field4=";
//      postStr += String(relaypin);
//      postStr += "\r\n\r\n\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
   
  }
    client.stop();
  
}
