#include <Arduino.h>
#include <U8x8lib.h>
#include <Servo.h>

//display
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

//pins
const int ThermistorPin = 0;
const int UpMinPin = 2;
const int DownMinPin = 3;
const int UpMaxPin = 4;
const int DownMaxPin = 5;
const int ServoPin = 9;

//buttons
int i=2;

//Servo vars
int pos = 0;
Servo myservo;

//temparature calculation vars
int Vo;
float R1 = 10000;
float logR2, R2, Tk, currentTemp;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
  Serial.begin(9600);
  myservo.attach(ServoPin);  // attaches the servo on pin 9 to the servo object
  u8x8.begin();
  u8x8.setPowerSave(0);
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
  dtostrf(currentTemp, 3, 1, curTempString);
  char curLine[10];
  snprintf(curLine, sizeof curLine, "%s%s", "Now:", curTempString);
  u8x8.drawString(0, 2, curLine);
}

void testGetTemp() {
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  Tk = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  currentTemp = Tk - 273.15;
  Serial.println(currentTemp);
}

void testServo(){
 for(pos = 0; pos < 180; pos += 1) 
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
