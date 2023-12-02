#include <Arduino.h>
#include <U8x8lib.h>
#include <Servo.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define PIN_DHT22 2

//display
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

//pins
const int Button2 = 2; //left up
const int Button3 = 3; //right up
const int Button4 = 4; //right down
const int Button5 = 5; //left down

const int PinGnd=6;
const int PinVcc=7;

const int ServoPin = 9;

//buttons
int i=2;

//Servo vars
int pos = 0;
Servo myservo;

//temparature calculation vars
DHT dht(PIN_DHT22, DHTTYPE);
float humidity, temperature;

void setup() {
  Serial.begin(9600);
  myservo.attach(ServoPin);  // attaches the servo on pin 9 to the servo object
  u8x8.begin();
  u8x8.setPowerSave(0);    
  dht.begin();

  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);
  pinMode(Button4, INPUT);
  pinMode(Button5, INPUT);
  
  pinMode(PinGnd, OUTPUT);
  pinMode(PinVcc, OUTPUT);
  digitalWrite(PinGnd, LOW);
  digitalWrite(PinVcc, HIGH);
  }

void loop() {
  testServo();
  testDisplay();
  testButtons();
  testGetTemp();
}

void testButtons(){
for(i = 2; i < 6; i += 1) 
 {
  if (digitalRead(i) == HIGH) {
    Serial.println(i);
  }
 }
}

void testDisplay(){
  u8x8.setFont(u8x8_font_courB18_2x3_f);
  char curTempString[3];
  dtostrf(temperature, 3, 1, curTempString);
  char curLine[10];
  snprintf(curLine, sizeof curLine, "%s%s", "Now:", curTempString);
  u8x8.drawString(0, 2, curLine);
}

void testGetTemp() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
}

void testServo(){
 for(pos = 0; pos < 10; pos += 1) 
 {                                 
    myservo.write(pos);            
    delay(15);                     
 }
 for(pos = 180; pos>=1; pos-=1)    
 {
    myservo.write(pos);            
    delay(15);                     
 }  
}
