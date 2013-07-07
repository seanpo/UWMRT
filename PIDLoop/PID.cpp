

PID::PID(int KP, int KI, int KD) {
  _goal = 0;
  _KP = KP;
  _KI = KI;
  _KD = KD;
} 

PID::init() {
  _proportional = 0;
  _integral = 0;
  _derivative = 0;
  _oldValue = 0;
  _oldError = 0;
}

void PID::setGoal(int goal) {
  _goal = goal;
  _proportional = 0;
  _integral = 0;
  _derivative = 0;
}

int PID::run(int value) {
  int error = value - _goal;
  
  _proportional = error;
  _integral += error;
  _derivative = _oldError - error;

  _oldValue = value;
  _oldError = error;  

  return _KP*error + _KI*_integral + _KD*_derivative
}
