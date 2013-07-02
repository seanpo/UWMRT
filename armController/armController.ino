#include "motorControl.h"
#define MAX_EDGE_COUNT 10000
#define s 115
#define e 101
#define c 99

MotorControl *motor;
int incomingByte;

void setup() {
  motor = new MotorControl(12, 11, 10, 9, MAX_EDGE_COUNT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == s) {
      Serial.println("STOPING"); 
      motor->stop();
    } else if (incomingByte == e) {
      Serial.println("EXTENDING"); 
      motor->extend();
    } else if (incomingByte == c) {
      Serial.println("CONTRACTING"); 
      motor->contract();
    }
  }
}
