#include <Arduino.h>
#include <U8x8lib.h>

//display
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

//pins
const int ThermistorPin = 0;
const int UpMinPin = 2;
const int DownMinPin = 3;
const int UpMaxPin = 4;
const int DownMaxPin = 5;
const int ServoPin = 9;

//settings
int maxTemp = 25;
int minTemp = 20;

//temparature calculation vars
int Vo;
float R1 = 10000;
float logR2, R2, Tk, currentTemp;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


void setup(void)
{
  pinMode(UpMinPin, INPUT);
  pinMode(DownMinPin, INPUT);
  pinMode(UpMaxPin, INPUT);
  pinMode(DownMaxPin, INPUT);

  Serial.begin(9600);
  u8x8.begin();
  u8x8.setPowerSave(0);
}


void loop(void)
{
  Serial.println("loopi");
  get_temp();
  get_user_input();
  draw_overview();
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
  if (digitalRead(UpMinPin) == HIGH) {
    if (minTemp < maxTemp) {
      minTemp++;
    }
  }
  if (digitalRead(DownMinPin) == HIGH) {
    minTemp--;
  }

  //max buttons
  if (digitalRead(UpMaxPin) == HIGH) {
    maxTemp++;
  }
  if (digitalRead(DownMaxPin) == HIGH) {
    if(maxTemp>minTemp){
    maxTemp--;
    }
  }
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
