#include "Arduino.h"
#include "PID.h"
#include "serialRead.h"

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

#define MAX_PULSE 2000
#define MIN_PULSE 1000

#define MAX 1000
#define MIN 0

MegaEncoderCounter megaEncoderCounter(4);  // Initialize with 1x mode

// The encoder logic is a look-up table deried from http://letsmakerobots.com/node/24031
int QEM [16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};       // Quadrature Encoder Matrix
int QEM_index = 0;
long int encTick_array[NUM_ENC];
int led = 13;

SoftwareServo motors[NUM_MTR];

PID *pid;
int KP = 80;
int KI = 10;
int KD = 0;
int goal = 0;
int pidCmd = 0;

void setup() {       
  // Interrupts
  noInterrupts();               
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  OCR3A  = 228;              // compare match register 16MHz/1/70kHz
  TCCR3B |= (1 << WGM32);    // CTC mode
  TCCR3B |= (1 << CS30);     // 1 prescaler 
  TIMSK3 |= (1 << OCIE3A);   // enable timer compare interrupt
  interrupts();
  
  // Serial
  Serial.begin (115200);
  Serial.println();

  // I/O
  motors[0].attach(2);
  motors[0].setMaximumPulse(MAX_PULSE);
  motors[0].setMinimumPulse(MIN_PULSE);
    
  pinMode(led, OUTPUT);   
  
  // PID
  pid = new PID(KP,  KI,  KD,  MAX,  MIN);
}

ISR(TIMER3_COMPA_vect){
  for (int i=0; i<1; i++){
    encTick_array[i] = megaEncoderCounter.XAxisGetCount();  // have to change library later
    pidCmd = pid->run(encTick_array[i]);
    motors[i].write(pidCmd + MIN_PULSE);
    SoftwareServo::refresh();
  }    
}

// the loop routine runs over and over again forever:
void loop() {
    if ( Serial.available()) {
      char ch = Serial.read();
      switch(ch) {
        case '1':
          KP = serialReadInt();
          KI = serialReadInt();
          KD = serialReadInt();
          pid->setConstants(KP, KI, KD);
          break;
        case '2':
          goal = serialReadInt();
          pid->setGoal(goal);
          break;
      }
    }

  Serial.print("Ticks: ");
  Serial.print(encTick_array[0]);
  Serial.print(" Rev: ");
  Serial.println(pidCmd);
  Serial.print(" ");
  Serial.println(encTick_array[0]/TRUE_PPR);
}
