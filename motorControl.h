#ifndef motorControl_h
#define motorControl_h

#include "Arduino.h"

#define MAX_EDGE_COUNT 999999 // TODO: Update this number.
#define AVG_ENCODER_VAL 315
#define MAX_NO_CHANGE_COUNT 3

enum State {EXTEND, CONTRACT, STOP};

class MotorContol {
  public:
    MotorControl(int extendPinA, int extendPinB, int contractPinA, int contractPinB, int encoderPin);
    MotorControl(int extendPinA, int extendPinB, int contractPinA, int contractPinB);

    void updateEncoder();

    bool extend();
    bool contract();
    void stop();
  private: 
    void _init(int extendPinA, int extendPinB, int contractPinA, int contractPinB, int encoderPin);

    void _extend();
    void _contract();

    bool _digitizeEncoder(int encoderIn);

    // Pin variables
    int _extendPinA, _extendPinB, _contractPinA, _contractPinB;
    int _encoderPin;

    // State variables
    int _edgeCount;
    int _noChangeCount;
    bool _prevEncoderVal;
    bool _useEncoder;
    State _state;
};

#endif

