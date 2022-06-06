/* This code is to be used on the Arduino acting as Transmitter.
 *  Required hardware is 2 Joysticks.
 */

#include <SPI.h>
#include <DDCONlib.h>

//Define parameters needed for radio
RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

  //==================Define Buttons and Payload Structure=========================

  RightPackage data;

// ================= Set up components =================
//Joystick Setup 
const int VRx = A0;
const int VRy = A1;

//============ Radio Setup ===========
const uint8_t rDelay = 7; //this is based on 250us increments, 0 is 250us so 7 is 2 ms
const uint8_t rNum = 5; //number of retries that will be attempted 

void controllerRadioSetup(){
    myRadio.begin();
    myRadio.setChannel(0x34);
    myRadio.setRetries(rDelay,rNum);
    myRadio.setPALevel(RF24_PA_LOW);
    myRadio.setDataRate( RF24_2MBPS ) ;
    myRadio.openWritingPipe( addresses[0]);
    myRadio.setPayloadSize(sizeof(data));
    myRadio.stopListening();
}
    

void setup(){
    Serial.begin(9600);
    
    //Set pins for joystick to input  
    pinMode(VRx, INPUT);
    pinMode(VRy, INPUT);

    //Set pins for buttons to input
    buttonSetup();


    delay(1000);
    randomSeed(analogRead(0));

    // Set up the radio communication 
    controllerRadioSetup();

    //Set up controller id:
    data.id = 1;
}

void loop(){
  delay(50);
  
//Read current state of joystick
  data.joystick.xPos = map(analogRead(VRx), 0, 1023, 0, 255);
  data.joystick.yPos = map(analogRead(VRy), 0, 1023, 0, 255);

  for (int thisPin=2; thisPin < 10;  thisPin++ ){
        switch(thisPin){
          case 2:
          data.buttons.A = digitalRead(thisPin);
          break;

          case 3:
          data.buttons.B = digitalRead(thisPin);
          break;

          case 4:
          data.buttons.X = digitalRead(thisPin);
          break;

          case 5:
          data.buttons.Y = digitalRead(thisPin);
          break;

          case 6:
          data.buttons.Jsb = digitalRead(thisPin);
          break;

          case 7:
          data.buttons.Plus = digitalRead(thisPin);
          break;

          case 8:
          data.buttons.ZR = digitalRead(thisPin);
          break;

          case 9:
          data.buttons.RT = digitalRead(thisPin);
          
          break;
          
          default:
          Serial.println("ERROR");
        }
    }

  
if (!myRadio.write(&data, sizeof(data)))
  {  //send data and remember it will retry if it fails
    delay(random(5, 20)); //as another back up, delay for a random amount of time and try again
    if (!myRadio.write(&myRadio, sizeof(myRadio)))
      {
      Serial.println("Transmission failed");
      delay(random(5,20));
      
      }  
  }

}
       
