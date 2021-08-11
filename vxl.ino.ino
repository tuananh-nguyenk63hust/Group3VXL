#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define DHTPIN 9
#define DHTTYPE DHT11
#define LIGHT 3
DHT dht(DHTPIN,DHTTYPE);
LiquidCrystal_I2C lcd(0x3F,16,2);
uint8_t fan=0;
uint8_t led=0;
int fan_get=-1;
int led_get=-1;
String serverName="http://42.115.245.190";
long long timestamp_post=0;
long long timestamp_get=0;

void setup() {
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED)
    {
      Serial.print("Checking... Ok");
      WiFiClient client;
      HTTPClient http;
    }
    lcd.init();
    lcd.backlight();
    //lcd.setCursor(2,0);
    //lcd.print("Vi xu ly N3");
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    dht.begin();
    
    
}
void sendAPI( int tmp, int as, int led, int fan)
{
  http.begin(serverName,"");
  http.addHeader("Content-Type", "application/json");
  String jsonString = "{\"YL69\":"+led+",\"fan\":"+fan+",\"as\":"+as+",\"temp\":"+temp+"}";
  int httpResponseCode = http.POST(jsonString);
  if (httpResponseCode>0)
  {
    String payload=http.getString();
    Serial.println(payload);
  }
}
void getAPI()
{
    http.begin(serverName,"");
    int res_status=http.GET();
    if (res_status>0){
      String payload=http.getString();
      fan_get=payload["fan"];
      led_get=payload["led"];
    }
}
void loop() {
    delay(1000);
    getAPI();
    if (millis()-timestamp_get>=120000)
    {
      timestamp_get=millis();
      getAPI();
    }
    float temp=dht.readTemperature();
    lcd.setCursor(2,0);
    lcd.print("Temp:");
    lcd.setCursor(7,0);
    lcd.print(temp);
    uint8_t light=analogRead(LIGHT); // use analog
    if (ligh>=3) 
    {
      led=1;
    }
    else led=0;
    if (temp >25)
    {
      fan=1;
    }
    else
    {
      fan=0;
    }
    if (led_get!=-1)
    {
      led=led_get;
    }
    if (fan_get!=-1)
    {
      fan=fan_get;
    }
    digitalWrite(3,led);
    digitalWrite(4,fan);
    if (millis()-timestamp_post>=120000)
    {
      timestamp_post=millis();
      sendAPI(temp,light,led,fan);
      
    }
} 
