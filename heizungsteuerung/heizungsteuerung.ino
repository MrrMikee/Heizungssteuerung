#include <Arduino.h>
#include <U8x8lib.h>
#include <Servo.h>


//display
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

//pins
const int ThermistorPin = 0;
const int Button2 = 2;
const int Button3 = 3;
const int Button4 = 4;
const int Button5 = 5;

const int ServoPin = 9;

const int PinGnd=6;
const int PinVcc=7;

//settings
int maxTemp = 25;
int minTemp = 20;

//temparature calculation vars
int Vo;
float R1 = 10000;
float logR2, R2, Tk, currentTemp;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

//Servo vars
int pos = 0;
Servo heaterServo;

void setup(void) {
  Serial.begin(9600);

  u8x8.begin();
  u8x8.setPowerSave(0);

  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);
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
  Serial.println(currentTemp, pos);
  delay(2000);
}


void get_temp() {
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  Tk = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  currentTemp = Tk - 273.15;
}


void get_user_input() {
  //min buttons
  if (digitalRead(Button2) == HIGH) {
    if (minTemp < maxTemp) {
      minTemp++;
    }
  }
  if (digitalRead(Button3) == HIGH) {
    minTemp--;
  }

  //max buttons
  if (digitalRead(Button5) == HIGH) {
    maxTemp++;
  }
  if (digitalRead(Button4) == HIGH) {
    if(maxTemp>minTemp){
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
  char maxLine[10];
  snprintf(maxLine, sizeof maxLine, "%s%d", "Max: ", maxTemp);
  u8x8.drawString(0, 0, maxLine);

  u8x8.setFont(u8x8_font_courB18_2x3_f);
  char curTempString[3];
  dtostrf(currentTemp, 3, 1, curTempString);
  char curLine[10];
  snprintf(curLine, sizeof curLine, "%s%s", "Now:", curTempString);
  u8x8.drawString(0, 2, curLine);

  u8x8.setFont(u8x8_font_7x14B_1x2_f);
  char minLine[10];
  snprintf(minLine, sizeof minLine, "%s%d", "Min: ", minTemp);
  u8x8.drawString(0, 5, minLine);
}
