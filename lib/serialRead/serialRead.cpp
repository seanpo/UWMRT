#include "serialRead.h"
#include "Arduino.h"

int serialReadInt(){
    String strNum;
    int counter = 0;
	
	while (Serial.available() <= 0);
    while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill 
      if (Serial.available() > 0) {
        char c = Serial.read();  //gets one byte from serial buffer
        strNum += c; //makes the string readString
      } 
    }
    return strNum.toInt();  
}