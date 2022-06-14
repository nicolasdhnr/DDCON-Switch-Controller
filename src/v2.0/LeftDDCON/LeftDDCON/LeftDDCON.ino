  /* This code is to be used on the Arduino acting as Transmitter.
   *  Required hardware is 2 Joysticks.
   *  **************************LEFT JOYSTICK*******************
   */
  
  #include <SPI.h>  
  #include "RF24.h"
  #include <DDCONlib.h>
  
  //Define parameters needed for radio
  RF24 myRadio (2, SS);
  byte addresses[][6] = {"0"};
  //================== Timer Set up =============================
int counter = 0;
unsigned long val;
int addr = 0;

//==================Define Payload structure==========================

Joystick joystick;
Package data;
Buttons button;

//=================== Nintendo Switch Controls ===============================
      
switchCommand current;

  const int VRx = A0;
  const int VRy = A1;
  //============ Radio Setup ===========
  const uint8_t rDelay = 7; //this is based on 250us increments, 0 is 250us so 7 is 2 ms
  const uint8_t rNum = 5; //number of retries that will be attempted 
  
  void radioSetup(){
     myRadio.begin();  
      myRadio.setChannel(125); // 
      myRadio.setRetries(rDelay,rNum);
      myRadio.setPALevel(RF24_PA_LOW);
      myRadio.setDataRate( RF24_2MBPS ) ; 
      myRadio.openWritingPipe( addresses[0]);
      myRadio.setPayloadSize(sizeof(data));
      myRadio.stopListening();
  }
  
      
  
  void setup(){
      
      //Set pins for joystick to input
      
      pinMode(VRx, INPUT);
      pinMode(VRy, INPUT);

      //Set button Pins to INPUT_PULLUP, allowing the use of the built-in 20k resistors. 
      leftButtonSetup();
      
      Serial.begin(9600);
      delay(1000);

      //Random seed for the random delay.
      randomSeed(analogRead(0));

    // Set up the radio communication 
    radioSetup();
}

void loop(){
  delay(50);
  //Left Joystick 
  joystick.xPos = map(analogRead(VRx), 0, 1023, 0, 255);
  joystick.yPos = map(analogRead(VRy), 0, 1023, 0, 255);

 
  button = readLeftPins();
  
  data.id = 2;
  data.joystick = joystick;
  data.buttons = button;
   printRightPackageInfo(data);
if (!myRadio.write(&data, sizeof(data))){  //send data and remember it will retry if it fails
    delay(random(5, 20)); //as another back up, delay for a random amount of time and try again
    if (!myRadio.write(&myRadio, sizeof(myRadio))){
      delay(random(5,20));
      
      }  
     }

}
       
