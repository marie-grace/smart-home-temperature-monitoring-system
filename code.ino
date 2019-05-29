//include libraries
#include<DHT.h>      
#include<ESP8266WiFi.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
//Define constants and pins
String apiKey = "KHAZEV7JTIOORFIJ";
const char* ssid = "wifi";
const char* password =  "unknown974";
const char* server = "api.thingspeak.com";

LiquidCrystal_I2C lcd(0x27, 20, 4);
#define dht_dpin 0
#define DHTTYPE DHT11
#define fan_pwm 14
int potPin = A0;       
int val = 0;   
DHT dht(dht_dpin,DHTTYPE);
byte degree[8] = 
              {
                0b00011,
                0b00011,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000
              };
WiFiClient client;
void setup()
{
 pinMode(fan_pwm, OUTPUT);
 dht.begin();
 lcd.begin(20,4); 
 lcd.init();
 lcd.backlight();
 lcd.setCursor(2, 0);
 lcd.createChar(1, degree);
 lcd.clear();
 lcd.print("   Fan Speed  ");
 lcd.setCursor(0,1);
 lcd.print("  Controlling ");
 delay(2000);
 lcd.clear();
 lcd.print("Circuit Digest ");
 delay(2000);
 lcd.clear();
 WiFi.begin(ssid, password);
 
  lcd.clear();
  lcd.print("Connecting to ");
  lcd.print(ssid);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  lcd.clear();
  lcd.print("WiFi connected!     ");
}
void loop()
{
  val = analogRead(potPin);                
  lcd.write(val);                  
  float temp = dht.readTemperature();
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
    
           postStr +="&field1=";
           postStr += String(temp);
           postStr += "\r\n\r\n";
 
    client.print("POST /update HTTP/1.1\n"); 
    client.print("Host: api.thingspeak.com\n"); 
    client.print("Connection: close\n"); 
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
    client.print("Content-Type: application/x-www-form-urlencoded\n"); 
    client.print("Content-Length: "); 
    client.print(postStr.length()); 
    client.print("\n\n"); 
    client.print(postStr);
    lcd.setCursor(0,1);
    lcd.print("Temperature:");
    lcd.print(temp);
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(0,2);
    if(temp <=25 )
      { 
        digitalWrite(fan_pwm, 0);
        lcd.print("Fan OFF             ");
        delay(100);
      }
      
      else if(temp<=27)
      {
        digitalWrite(fan_pwm, 51);
        lcd.print("Fan Speed: 20%      ");
        delay(100);
      }
      
       else if(temp<=29)
      {
        digitalWrite(fan_pwm, 102);
        lcd.print("Fan Speed: 40%      ");
        delay(100);
      }
      
       else if(temp<=30)
      {
        digitalWrite(fan_pwm, 153);
        lcd.print("Fan Speed: 60%      ");
        delay(100);
      }
      
      else if(temp<=32)
      {
        digitalWrite(fan_pwm, 204);
        lcd.print("Fan Speed: 80%       ");
        delay(100);
      }
       else if(temp>33)
      {
        digitalWrite(fan_pwm, 255);
        lcd.print("Fan Speed: 100%      ");
        delay(100);
    } 
  delay(3000);
}
client.stop();
       
  // thingspeak needs minimum 15 sec delay between updates
  delay(20000); 
}
