#include "PID.h"
#include "Arduino.h"

PID::PID(int KP, int KI, int KD, int MAX, int MIN) {
  init();
  _KP = KP/100.0;
  _KI = KI/100.0;
  _KD = KD/100.0;
  _MAX = MAX;
  _MIN = MIN;
} 

void PID::init() {
   _goal = 0;
  _proportional = 0;
  _integral = 0;
  _derivative = 0;
  _oldValue = 0;
  _oldError = 0;
}

void PID::setGoal(int goal) {
  init();
  _goal = goal;
}

int PID::run(int value) {
  int error = _goal - value;
  
  _proportional = error;
  _integral += error;
  _derivative = _oldError - error;

  _oldValue = value;
  _oldError = error;  
  
  float response = _KP*error + _KI*_integral + _KD*_derivative;
  if (response < _MAX && response > _MIN) {
    return response;
  } else if (response > _MAX) {
    return _MAX;
  } else {
    return _MIN;
  }
}

void PID::print() {
  Serial.print("proportional: ");
  Serial.print(_proportional);
  Serial.print(", integral: ");
  Serial.print(_integral);
  Serial.print(", derivative: ");
  Serial.println(_derivative);
}
