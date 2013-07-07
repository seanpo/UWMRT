#include "serialRead.h"
#include "PID.h"

#include <cstdlib>
#define KP 80
#define KI 20
#define KD 40
#define MAX 1000
#define MIN -1000

PID *pid;
int incomingByte;
int distance = 0;

void setup() {
  pid = new PID(KP,KI,KD,MAX,MIN); 
  Serial.begin(9600);
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
