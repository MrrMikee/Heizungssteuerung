#include <Arduino.h>
#include <U8x8lib.h>
#include <Servo.h>
#include "DHT.h"


#define DHTTYPE DHT22
#define PIN_DHT22 2


//display
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

//pins
const int ThermistorPin = 0;
//const int Button2 = 12;
//const int Button3 = 13;
const int Button4 = 4;
const int Button5 = 5;

const int ServoPin = 9;

const int PinGnd=6;
const int PinVcc=7;

//settings
int maxTemp = 19;
int minTemp = 18;

//Temparature vars
DHT dht(PIN_DHT22, DHTTYPE);
float currentTemp;

//Servo vars
int pos = 0;
Servo heaterServo;

//Display vars
char maxLine[10];
char curTempString[3];
char minLine[10];

void setup(void) {
  Serial.begin(9600);

  u8x8.begin();
  u8x8.setPowerSave(0);
  dht.begin();

//  pinMode(Button2, INPUT);
//  pinMode(Button3, INPUT);
  pinMode(Button4, INPUT);
  pinMode(Button5, INPUT);

  heaterServo.attach(ServoPin);

  //Use these pins as additional vcc and gnd pins
  pinMode(PinGnd, OUTPUT);
  pinMode(PinVcc, OUTPUT);
  digitalWrite(PinGnd,LOW);
  digitalWrite(PinVcc,HIGH);
}

void loop(void)
{
  get_temp();
  get_user_input();
  calc_servo_pos();
  draw_overview();
  Serial.print(" Temparature: ");
  Serial.print(currentTemp);
  Serial.print(" Position: ");
  Serial.print(pos);
  Serial.println();
  delay(2000);
}


void get_temp() {
  currentTemp = dht.readTemperature();
}


void get_user_input() {
  //min buttons
//  if (digitalRead(Button2) == HIGH) {
//    Serial.println("Button2");
//    if (minTemp < maxTemp) {
//      minTemp++;
//    }
//  }
//  if (digitalRead(Button3) == HIGH) {
//    Serial.println("Button3");
//    minTemp--;
//  }

  //max buttons
  if (digitalRead(Button5) == HIGH) {
    Serial.print("Button5 is pressed ");
    maxTemp++;
    // also increase lower temp, because button is broken
    minTemp++;
  }
  if (digitalRead(Button4) == HIGH) {
    Serial.print(" Button4 is pressed ");
    if(maxTemp>minTemp){
    // also decrease lower temp, because button is broken
    minTemp--;
    maxTemp--;
    }
  }
}

void calc_servo_pos(){
  if(pos<=90){ //off
    if(currentTemp<minTemp){
      pos=180;
    }
  }
  if(pos>90){ //on
    if(currentTemp>maxTemp){
      pos=0;
    }
  }
  heaterServo.write(pos);
}

void draw_overview() {
  u8x8.setFont(u8x8_font_7x14B_1x2_f);
  snprintf(maxLine, sizeof maxLine, "%s%d", "Max: ", maxTemp);
  u8x8.drawString(0, 0, maxLine);

  u8x8.setFont(u8x8_font_courB18_2x3_f);
  dtostrf(currentTemp, 3, 1, curTempString);
  char curLine[10];
  snprintf(curLine, sizeof curLine, "%s%s", "Now:", curTempString);
  u8x8.drawString(0, 2, curLine);
  
  u8x8.setFont(u8x8_font_7x14B_1x2_f);
  snprintf(minLine, sizeof minLine, "%s%d", "Min: ", minTemp);
  u8x8.drawString(0, 5, minLine);
}
