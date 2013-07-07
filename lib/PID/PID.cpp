#include "PID.h"
#include "Arduino.h"

PID::PID(int KP, int KI, int KD, int MAX, int MIN) {  
  _MAX = MAX;
  _MIN = MIN;
  init();
  setConstants(KP, KI, KD);
} 

void PID::print() {
  Serial.print("proportional: ");
  Serial.print(_proportional);
  Serial.print(", integral: ");
  Serial.print(_integral);
  Serial.print(", derivative: ");
  Serial.println(_derivative);
}

void PID::setConstants(int KP, int KI, int KD) {
  _KP = KP/100.0;
  _KI = KI/100.0;
  _KD = KD/100.0;
}

void PID::setGoal(int goal) {
  init();
  _goal = goal;
}

int PID::run(int value) {
  int error = _goal - value;
  
  _proportional = error;
  _integral = _integral + error;
  _derivative = error - _oldError;

  _oldValue = value;
  _oldError = error;  
  
  float response = _KP*error + _KI*_integral + _KD*_derivative;
  return max(min(response, _MAX),_MIN);
}

void PID::init() {
   _goal = 0;
  _proportional = 0;
  _integral = 0;
  _derivative = 0;
  _oldValue = 0;
  _oldError = 0;
}