#include "PID.h"
#include <cstdlib>
#define KP 80
#define KI 40
#define KD 40
#define MAX 200
#define MIN -200

PID *pid;
int incomingByte;
int distance = 0;

int serialReadInt(){
    String strNum;
    int counter = 0;

    while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill 
      if (Serial.available() >0) {
        char c = Serial.read();  //gets one byte from serial buffer
        strNum += c; //makes the string readString
      } 
    }
    return strNum.toInt();  
}

void setup() {
  pid = new PID(KP,KI,KD,MAX,MIN); 
  Serial.begin(9600);
  setGoal();
}

void setGoal() {
  Serial.println("What is your goal?");
  while(Serial.available() <= 0) {
  }
  incomingByte = serialReadInt();
  pid->setGoal(incomingByte);
}


void loop() {
  if (Serial.available() > 0) {
    incomingByte = serialReadInt();
    if (incomingByte == 1) {
      setGoal();
    } else if (incomingByte == 2){
      pid->print();
    } else {
      distance += pid->run(distance);
      Serial.println(distance);
    }
  }
}
