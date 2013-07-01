#include "motorControl.h"

const int buttonPin = 2;

MotorControl motor(12,11,19,9,0);

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT); 
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    motor.extend();
  } else {  
    motor.contract();
  }
  
  motor.updateEncoder();
}
