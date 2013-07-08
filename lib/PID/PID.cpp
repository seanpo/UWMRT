#include "PID.h"
#include "Arduino.h"

PID::PID(int KP, int KI, int KD, int WORKING_BAND) {  
  _WORKING_BAND = WORKING_BAND;
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
  
  _proportional = _KP*error;
  _integral = _integral + _KI*error;
  _derivative = _KD*(error - _oldError);
  
  _oldError = error;  
  return max(min(error + _integral + _derivative, _WORKING_BAND),-1*_WORKING_BAND);
}

void PID::init() {
   _goal = 0;
  _proportional = 0;
  _integral = 0;
  _derivative = 0;
  _oldError = 0;
}