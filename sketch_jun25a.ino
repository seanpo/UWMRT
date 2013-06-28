// THE TRANSISTORS 
// Yellow wires
const int turnLeftAPin = 12; 
const int turnLeftBPin = 11; 
// White wires
const int turnRightAPin = 10; 
const int turnRightBPin = 9;
// Yello Wire from the motor
const int encoderPin = 0;
const int buttonPin = 2;

const int MAX_ENCODER_VAL = 999999;
const int ENCODER_AVG = 315;
const int MAX_NO_CHANGE_COUNT = 3;

const int LEFT_MOTOR_STATE = 2;
const int RIGHT_MOTOR_STATE = 1;
const int STOP_MOTOR_STATE = 0;


int encoderVal = 0; // encoderVal = 0 refers to the state when the arm is fully closed.
int prevEncoderState = 0;
int buttonState = 0;
int motorState = 0;
// This count keeps track of the number of pulses of the encoder that dont have a change. 
// Using this we can get rid of issues with encoder errors.
int noChangeCount = 0;

// Function definitions
void stopMotor();
void runLeft();
void runRight();
void readEncoder();

void setup() {
  Serial.begin(9600);
  pinMode(turnLeftAPin, OUTPUT); 
  pinMode(turnLeftBPin, OUTPUT); 
  pinMode(turnRightAPin, OUTPUT); 
  pinMode(turnRightBPin, OUTPUT); 
  pinMode(buttonPin, INPUT); 
}

void loop(){
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    if (encoderVal > 0) { 
      runRight(); 
    } else {
      stopMotor(); 
    }
  }  
  else {  
    if (encoderVal < MAX_ENCODER_VAL) {
      runLeft();
    } else {
      stopMotor(); 
    }
  }
  
  runLeft();
  delay(1000);
  runRight();
  delay(1000);  
  readEncoder();
}

void readEncoder() {
  int encoderState = analogRead(encoderPin);
 
  if ((encoderState < ENCODER_AVG && prevEncoderState > ENCODER_AVG) || (encoderState > ENCODER_AVG && prevEncoderState < ENCODER_AVG)) {
    if (motorState == LEFT_MOTOR_STATE && encoderVal < MAX_ENCODER_VAL) {
      encoderVal++;
    } else if (motorState == RIGHT_MOTOR_STATE && encoderVal > 0) {
      encoderVal--;
    }
    prevEncoderState = encoderState;
  } else {
    noChangeCount++;
    if (noChangeCount >= MAX_NO_CHANGE_COUNT && (encoderVal < MAX_ENCODER_VAL || encoderVal > 0)) {
      noChangeCount = 0;
      if (motorState = LEFT_MOTOR_STATE) {
        encoderVal++;
      } else if(motorState = RIGHT_MOTOR_STATE) {
        encoderVal--;
      }
    } 
  }
  
  Serial.println(encoderVal); 
}

void stopMotor() {
  digitalWrite(turnLeftAPin, LOW);
  digitalWrite(turnLeftBPin, LOW);
  digitalWrite(turnRightAPin, LOW);
  digitalWrite(turnRightBPin, LOW);
  motorState = STOP_MOTOR_STATE;
}

void runLeft(){
  digitalWrite(turnLeftAPin, HIGH);
  digitalWrite(turnLeftBPin, HIGH);
  digitalWrite(turnRightAPin, LOW);
  digitalWrite(turnRightBPin, LOW);
  motorState = LEFT_MOTOR_STATE;
}

void runRight(){
  digitalWrite(turnRightAPin, HIGH);
  digitalWrite(turnRightBPin, HIGH);
  digitalWrite(turnLeftAPin, LOW);
  digitalWrite(turnLeftBPin, LOW);
  motorState = RIGHT_MOTOR_STATE;
}
