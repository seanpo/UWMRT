#include "serialRead.h"
#include "PID.h"

#include <cstdlib>
#define KP .8
#define KI .1
#define KD .1
#define WORKING_BAND 3000

PID *pid;
int incomingByte;
int distance = 0;

void setup() {
  pid = new PID(KP,KI,KD,WORKING_BAND); 
  Serial.begin(115200);
  setGoal();
}

void setGoal() {
  Serial.println("What is your goal?");
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
