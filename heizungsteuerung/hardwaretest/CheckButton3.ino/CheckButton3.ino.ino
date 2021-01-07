#include <Arduino.h>

//pins
const int Button3=13;
const int Button4=4; 
const int PinGnd=6;
const int PinVcc=7;

int i =0;
int j =0;

void setup(){
  Serial.begin(9600);
  
  pinMode(PinGnd, OUTPUT);
  pinMode(PinVcc, OUTPUT);
  digitalWrite(PinGnd, LOW);
  digitalWrite(PinVcc, HIGH);
  
  pinMode(Button3, INPUT);
}

void loop(){
//  Serial.println(analogRead(Button3));
  if (digitalRead(Button3) == HIGH) {
    i++;
  }
 if (digitalRead(Button4) == HIGH) {
    j++;
  }
  Serial.print(i);
  Serial.print(" , ");
  Serial.print(j);
  Serial.println();
}
