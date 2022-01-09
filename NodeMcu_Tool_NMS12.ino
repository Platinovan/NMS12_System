//Text size 2:12x16
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <ThingESP.h>

//definedn
#define OKLED D8
#define ERRLED D7
#define LED D5

//Objects
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
ThingESP8266 thing("CustomerLedps", "NodeMCU", "987654321");

//Functions
void correct();
void error();

//Variables
String WIFI_DIS = "Failed connection to ";
String WIFI_CON = "Connected to ";
String ssid = "Totalplay-DCA5";
String passwd = "DCA5C964HNZZjpcn";

//ints
int x;
int minX;
int counter = 0;

//boolean
bool connectd;

void setup(){
  Serial.begin(115200);
  
  //For indicator leds
  pinMode(OKLED, OUTPUT);
  pinMode(ERRLED, OUTPUT);
  pinMode(LED, OUTPUT);

  Wire.begin(D4, D3);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setRotation(90);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextWrap(false);
  oled.display();

  //For wifi settings
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);
  while(WiFi.status() != WL_CONNECTED && counter < 15){
    ++counter;
    oled.setTextSize(1);
    oled.setCursor(34, 15);
    oled.print("Connecting");
    oled.setTextSize(2);
    oled.setCursor(37, 24);
    for(int n = 0; n < 4; ++n){
      oled.print(".");
      delay(500);
      oled.display();
    }
    oled.clearDisplay();
  }
  counter = 0;
  oled.clearDisplay();
  oled.display();
  if(WiFi.status() == WL_CONNECTED){
    minX = -6 * (WIFI_CON.length() + ssid.length());
    connectd = true;
    thing.initDevice();
    delay(50);
  }else{
    minX = -6 * (WIFI_DIS.length() + ssid.length());
    connectd = false;
  }
}

void loop(){
  oled.clearDisplay();
  if(connectd){
    correct();
    oled.drawFastHLine(0, 16, 128, SSD1306_WHITE);
    oled.drawFastHLine(0, 48, 128, SSD1306_WHITE);
    oled.setTextSize(1);
    oled.setCursor(x, 28);
    oled.print(WIFI_CON + ssid);
    oled.display();
    
  }else{
    error();
    oled.drawFastHLine(0, 16, 128, SSD1306_WHITE);
    oled.drawFastHLine(0, 48, 128, SSD1306_WHITE);
    oled.setTextSize(1);
    oled.setCursor(x, 28);
    oled.print(WIFI_DIS + ssid);
    oled.display();
  }

  //Handle whatsapp query
  thing.Handle();

  //For text Scrolling
  x = x - 1;
  if(x < minX) x = oled.width();
}

//Handle Whatsapp query
String HandleResponse(String query){
  if(query == "led on"){
    digitalWrite(LED, HIGH);
    return "Led(D5) : ON";
  }else if(query == "led off"){
    digitalWrite(LED, LOW);
    return "LED(D5) : OFF";
  }else if(query == "led status"){
    return digitalRead(LED) ? "[STATUS] Led(D5) :: ON" : "[STATUS] Led(D5) :: OFF";     
  }else{
    return "\' " + query + " \'" + " :: " + "Unknown command";
  }
}

void correct(){
  digitalWrite(ERRLED, LOW);
  digitalWrite(OKLED, HIGH);
}

void error(){
  digitalWrite(OKLED, LOW);
  digitalWrite(ERRLED, HIGH);
}
