/* This code is to be used on the Arduino acting as Transmitter.
 *  Required hardware is 2 Joysticks.
 */

#include <SPI.h>  
#include "RF24.h"

//Define parameters needed for radio
RF24 myRadio (8, 9);
byte addresses[][6] = {"0"};

//==================Define Payload structure==========================
typedef struct Buttons{
  bool  A = 0;
  bool B = 0;
  bool X = 0;
  bool Y = 0;
}Buttons;

typedef struct Joystick{
int xPos;
int yPos ;
} Joystick;

Joystick joystick;

typedef struct Package
{
int id = 2;
Joystick joystick;
Buttons buttons ;
} Package;

//typedef struct buttonsJoystick{
//  int xPos;
//  int yPos;
//}buttonsJoystick;
//
//buttonsJoystick buttonsJoystick;

 Package data;

// ================= Set up components =================
//Joystick Setup 

const int VRx = A0;
const int VRy = A1;

//const int Buttons_X = A2;
//const int Buttons_Y = A3 ; 
// 
//Joystick rightJoystick;


//int buttonsXPos = 0;
//int buttonsYPos = 0;


//Mapped Joystick input variables 
int mapX1 = 0;
int mapY1 = 0;

//===============Define Functions needed inside loop ==================

//uint8_t findButton(buttonsJoystick joystick){
//  if (joystick.xPos >=950){
//    return "B";
//  }
//   else if (joystick.xPos <=150){
//    return "A";
//  }
//   else if (joystick.yPos >=950){
//    return "Y";
//  }
//   else if (joystick.yPos <=150){
//    return "X";
//  }
//}

//============ Radio Setup ===========
const uint8_t rDelay = 7; //this is based on 250us increments, 0 is 250us so 7 is 2 ms
const uint8_t rNum = 5; //number of retries that will be attempted 

void radioSetup(){
   myRadio.begin();  
    myRadio.setChannel(115); 
    myRadio.setRetries(rDelay,rNum);
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.setDataRate( RF24_250KBPS ) ; 
    myRadio.openWritingPipe( addresses[0]);
    myRadio.stopListening();
}
   

void setup(){
    
    //Set pins for joystick to input 
    
    pinMode(VRx, INPUT);
    pinMode(VRy, INPUT);
//    
//    pinMode(Buttons_X, INPUT);
//    pinMode(Buttons_Y, INPUT);
    
    Serial.begin(9600);
    delay(1000);

    // Set up the radio communication 
   radioSetup();
}

void loop(){

  //Left Joystick 
  joystick.xPos = map(analogRead(VRx), 0, 1023, 0, 255);
  joystick.yPos = map(analogRead(VRy), 0, 1023, 0, 255);
  //Right joystick
  //buttonsXPos = analogRead(Buttons_X);
  //buttonsYPos = analogRead(Buttons_Y);

  
  data.id = 1;
  data.joystick = joystick;
    
  myRadio.write(&data, sizeof(data)); 
  // Check data is correct by printing to Serial Monitor. 
  Serial.print("\nPackage:");
  Serial.print(data.id);
  Serial.print("\n");
  Serial.println(data.joystick.xPos);
  Serial.println(data.joystick.yPos);
  delay(50);
}
