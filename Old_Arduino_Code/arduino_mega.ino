#include "Arduino.h"
#include <WProgram.h>
#include <Wire.h>
#include <SoftwareServo.h>

#include <MegaEncoderCounter.h>
#include <digitalWriteFast.h>

#define PWM_PERCENT 25.5
#define TRUE_PPR 25344.0    // 48PPR * 132 Gear Ratio * 4 Quadature

#define PID_OFF 0
#define PID_ON  1

#define NUM_ENC 4
#define NUM_MTR 4
#define Tp 0.01
/*#define ENC_BASE 22
#define enc0PinA 22
#define enc0PinB 23
#define enc1PinA 24
#define enc1PinB 25
#define enc2PinA 26
#define enc2PinB 27
#define enc3PinA 28
#define enc3PinB 29
#define enc4PinA 30
#define enc4PinB 31
#define enc5PinA 32
#define enc5PinB 33
#define enc6PinA 34
#define enc6PinB 35
#define enc7PinA 36
#define enc7PinB 37

#define NUM_MTR 4
#define MTR_BASE 2
#define mtr_dfrntL 2
#define mtr_drearL 3
#define mtr_dfrntR 4
#define mtr_drearR 5
#define mtr_sfrntL 6
#define mtr_srearL 7
#define mtr_sfrntR 8
#define mtr_srearR 9

unsigned char encCurr_array[NUM_ENC];
unsigned char encPrev_array[NUM_ENC];*/


// Wheel loop gains
#define Kp_dv                   0.01
#define Kd_dv                   0.0
#define Ki_dv                   0.5

class PID {
  private:
  public:
    bool en;
    double Kp, Ki, Kd;
    double OUT_MAX, INT_MAX, DEAD_BAND;
    double e, e_old, de, ie;
    double sp, cv, out;

    PID (double _Kp, double _Ki, double _Kd) {
      Kp        = _Kp;
      Ki        = _Ki;
      Kd        = _Kd;
      OUT_MAX   =  999;
      INT_MAX   =  100;
      DEAD_BAND =  0;
    }
    
    void compute () {
      if (en) {
        e     = sp - cv;
        de    = (e - e_old) / Tp;
        ie    = max(-INT_MAX, min(ie + Ki * e * Tp, INT_MAX));
        out   = max(-OUT_MAX, min(Kp*e + Kd*de + ie, OUT_MAX));
        if (abs(e) <= DEAD_BAND)  out = 0;
      } else {
        out = 0;
      }
      e_old = e;
    }

};




PID                       dv01(Kp_dv, Ki_dv, Kd_dv);













long int encTick_array[NUM_ENC];
int led = 13;
MegaEncoderCounter megaEncoderCounter(4);  // Initialize with 1x mode

// The PID is based from http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/
double pidSet, pidCmd, pidOut, lastpidOut;
double cmdMax = 1, cmdMin = 0;  // of 2ms
double pidPro, pidInt, pidDif;
double kp = 1, ki = 1, kd = 1;
bool pidMode = false;
double pidSet_user = 0;

// The encoder logic is a look-up table deried from http://letsmakerobots.com/node/24031
int QEM [16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};       // Quadrature Encoder Matrix
int QEM_index = 0;

SoftwareServo motors[NUM_MTR];

void SetPIDMode(int Mode){
  if (Mode == PID_ON) {
    lastpidOut = pidOut;
    pidInt = pidCmd;
    if(pidInt > cmdMax) pidInt = cmdMax;     
    else if (pidInt < cmdMin) pidInt = cmdMin;
    pidMode = PID_ON;
  } else pidMode = PID_OFF;
  return;
}

void RunPID (long int sensor, double set_point){
    pidOut = sensor/TRUE_PPR;
    pidSet = set_point;
    
    if (pidMode){
      pidPro = pidSet - pidOut;               
      pidInt += (ki*pidPro);
      if(pidInt > cmdMax) pidInt = cmdMax;       
      else if (pidInt < cmdMin) pidInt = cmdMin;
      pidDif = (pidOut - lastpidOut);
      
      pidCmd = kp * pidPro + pidInt - kd * pidDif;
      if(pidCmd > cmdMax) pidCmd = cmdMax;
      else if(pidCmd < cmdMin) pidCmd = cmdMin;
   
      lastpidOut = pidOut;
    } else {
      pidCmd = pidSet; 
    }
  
}

void setup() {       

   /******************************
   * Inturupts
   ******************************/
  noInterrupts();               
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  OCR3A  = 228;              // compare match register 16MHz/1/70kHz
  TCCR3B |= (1 << WGM32);    // CTC mode
  TCCR3B |= (1 << CS30);     // 1 prescaler 
  TIMSK3 |= (1 << OCIE3A);   // enable timer compare interrupt
  interrupts();
  
  /******************************
   * Serial
   ******************************/
  Serial.begin (115200);
  Serial.println();
  
  /******************************
   * I/O
   ******************************/
  /*for (int i=0; i<NUM_ENC; i++){
   pinMode(ENC_BASE+i, INPUT); 
   digitalWrite(ENC_BASE+i, HIGH);  //Pull-up resistors
  }
  for (int i=0; i<NUM_MTR; i++){
    pinMode(MTR_BASE+i, OUTPUT);
    digitalWrite(MTR_BASE+i, LOW);
  }*/
  motors[0].attach(2);
  motors[0].setMaximumPulse(2000);
  motors[0].setMinimumPulse(1000);
  
  dv01.OUT_MAX = 90 * 0.4;
  dv01.DEAD_BAND = 1;
  dv01.en = true;
    
  pinMode(led, OUTPUT);   

  SetPIDMode(PID_ON);  
}

ISR(TIMER3_COMPA_vect){
  for (int i=0; i<1; i++){
    /*encPrev_array[i] = encCurr_array[i];
    encCurr_array[i] = digitalReadFast(ENC_BASE+(i*2))*2+digitalReadFast(ENC_BASE+(i*2)+1);  // Convert binary input to decimal
    QEM_index = encPrev_array[i]*4 + encCurr_array[i];
    encTicks_array[i] += QEM[QEM_index];*/
    
    encTick_array[i] = megaEncoderCounter.XAxisGetCount();  // have to change library later
    //RunPID (encTick_array[i], pidSet_user);
  dv01.compute();
    //analogWrite(2+i, pidCmd*PWM_PERCENT);
    motors[i].write(180*pidCmd);
    SoftwareServo::refresh();
  }    
}

// the loop routine runs over and over again forever:
void loop() {
    if ( Serial.available()) {
      char ch = Serial.read();
      switch(ch) {
        case '1':
          dv01.cv = 0.1;
          break;
        case '2':
          dv01.cv = 0.2;
          break;
        case '3':
          dv01.cv = 0.3;
          break;
        case '4':
          dv01.cv = 0.4;  
          break;
        case '5': 
          dv01.cv = 0.5;
          break;
      }
    }

  //Serial.print("Ticks: ");
  //Serial.print(encTick_array[0]);
  //Serial.print(" Rev: ");
  //Serial.println(pidCmd);
  //Serial.print(" ");
  //Serial.println(encTick_array[0]/TRUE_PPR);
}
