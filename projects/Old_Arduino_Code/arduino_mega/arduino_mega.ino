#include "Arduino.h"
#include "PID.h"
#include "serialRead.h"

#include <WProgram.h>
#include <Wire.h>
#include <SoftwareServo.h>

#include <MegaEncoderCounter.h>
#include <digitalWriteFast.h>

#define TRUE_PPR 25344.0    // 48PPR * 132 Gear Ratio * 4 Quadature

#define PID_OFF 0
#define PID_ON  1

#define NUM_ENC 4
#define NUM_MTR 4

#define MAX_PULSE 2000
#define MIN_PULSE 1000

#define PID_WORKING_BAND 3000

MegaEncoderCounter megaEncoderCounter(4);  // Initialize with 1x mode

long int encTick;
int led = 13;

SoftwareServo motors[NUM_MTR];

PID *pid;
int KP = 80;
int KI = 0;
int KD = 10;
int goal = 0;
int pidCmd = 90;

void printStats();
void setup();

ISR(TIMER3_COMPA_vect){
    encTick = megaEncoderCounter.XAxisGetCount();  // have to change library later
    pidCmd = map(pid->run(-1*encTick), 4*(-1*PID_WORKING_BAND), 4*(PID_WORKING_BAND), 0, 180);
    motors[0].write(pidCmd);
    SoftwareServo::refresh();
}  

// the loop routine runs over and over again forever:
void loop() {
    if ( Serial.available()) {
      char ch = Serial.read();
      delay(3);
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
        case '3':
          pidCmd = serialReadInt();
          break;
      }
    }
}

void printStats() {
  pid->print();
  Serial.print("Ticks: ");
  Serial.print(encTick);
  Serial.print(" Rev: ");
  Serial.println(pidCmd);
}

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
  pid = new PID(KP, KI, KD, PID_WORKING_BAND);
}


