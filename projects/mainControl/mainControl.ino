//Global Variables
//number of motors connected to the system
int maxNumMtrs = 8
//number of encoders connected to the system
//number of communication lines
//number of navigation devices

void setup() {

//create nav system
//test nav system
  //calibrate sensors if required
//create comm lines
//test comm lines
//create required motors
//test motors

//begin program
}

void loop() {

//always
  //check for any error signals
    //from the motors
    //from the navigation system
    //from the communcication lines

  
// FSM
  //check if system is in auto or manual control
  
  //auto
    //follow automatic control code
  
  //manual
    //await instructions from communication lines
  
  //on error
    //check error type
      //if error is repairable automatically run through repair routine
      //if error is fatal
        //depending on the device that is down do the following
          //motors are not working return a signal to base
          //navigation system is not working return a signal to base and
           //turn to manual control
          //communication system is having issues
            //if possible return a signal to base
            //if not either return to base or set to auto mode and carry on until
             //communications are back u
}
