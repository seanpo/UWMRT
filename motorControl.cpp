/******************************************************************** 
  MotorControl allows you to control the linear actuator. The last 
  parameter to the constructor is optional. If you decide to pass
  the encoderPin in, then information from it will be used to decide
  whether movement can be made or not.

  NOTE: This assumes that the starting position is fully contracted.
*********************************************************************/
#include "Arduino.h"

/******************************************************************** 
  Use this constructor when the encoder information is irrellevant
*********************************************************************/
MotorControl::MotorControl(int extendPinA, int extendPinB, int contractPinA, int contractPinB) {
  _init(extendPinA, extendPinB, contractPinA, contractPinB, -1);
  _useEncoder = false;
}

/******************************************************************** 
  Use this constructor when the encoder information is important
  The motor will stop moving when it is out of bounds.
*********************************************************************/
MotorControl::MotorControl(int extendPinA, int extendPinB, int contractPinA, int contractPinB, int encoderPin) {
  _init(extendPinA, extendPinB, contractPinA, contractPinB, encoderPin);
  _useEncoder = true;
}

void MotorControl::_init(int extendPinA, int extendPinB, int contractPinA, int contractPinB, int encoderPin) {
  _extendPinA = extendPinA;
  _extendPinB = extendPinB;
  _contractPinA = contractPinA;
  _contractPinB = contractPinB;
  _encoderPin = encoderPin;
  
  pinMode(extendPinA, OUTPUT);
  pinMode(extendPinB, OUTPUT);
  pinMode(contractPinA, OUTPUT);
  pinMode(contractPinB, OUTPUT);
  pinMode(encoderPin, INPUT);
  
  _state = STOP;
  _edgeCount = MAX_EDGE_COUNT;
  _prevEncoderVal = 0;
  _noChangeCount = 0;

  // Make motor completely contracted, while initializing edgeCount to 0.
  while(contract()) {
    _edgeCount--;
  }
}

void MotorControl::updateEncoder() {
  if (_useEncoder) {
    bool encoderVal = _digitizeEncoder(analogRead(_encoderPin));
   
    // This if block will be executed only when an edge has been detected,
    // or if no changes have been detected in MAX_NO_CHANGE_COUNT number of
    // reads.
    if (encoderVal != _prevEncoderVal || _noChangeCount >= MAX_NO_CHANGE_COUNT) {
      if (_state == EXTEND && _edgeCount < MAX_EDGE_COUNT) {
        _edgeCount++;
      } else if (_state == CONTRACT && _edgeCount > 0) {
        _edgeCount--;
      }
      _noChangeCount = 0;
    } else {
      _noChangeCount++;
    }
  }
}

void MotorControl::stop() {
  digitalWrite(_extendPinA, LOW);
  digitalWrite(_extendPinB, LOW);
  digitalWrite(_contractPinA, LOW);
  digitalWrite(_contractPinB, LOW);
  _state = STOP;
}

bool MotorControl::extend() {
  if (!useEncoder || _edgeCount < MAX_EDGE_COUNT) {
    _extend();
    return true;
  }
  return false;
}

bool MotorControl::contract() {
  if (!useEncoder || _edgeCount > 0) {
    _contract();
    return true;
  }
  return false;
}

void MotorControl::_extend() {
  digitalWrite(_extendPinA, HIGH);
  digitalWrite(_extendPinB, HIGH);
  digitalWrite(_contractPinA, LOW);
  digitalWrite(_contractPinB, LOW);
  _state = EXTEND;
}

void MotorControl::_contract() {
  digitalWrite(_extendPinA, LOW);
  digitalWrite(_extendPinB, LOW);
  digitalWrite(_contractPinA, HIGH);
  digitalWrite(_contractPinB, HIGH);
  _state = CONTRACT;
}

/******************************************************************** 
  Turns an encoder read value into a boolean depending on 
  AVG_ENCODER_VAL.
*********************************************************************/
bool MotorControl::_digitizeEncoder(int encoderIn) {
  return encoderIn > AVG_ENCODER_VAL;
}
