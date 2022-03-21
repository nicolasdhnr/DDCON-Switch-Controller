/* This code is to be used on the Arduino acting as Transmitter.
 *  Required hardware is 2 Joysticks.
 */

#include <SPI.h>  
#include "RF24.h"

//Define parameters needed for radio
RF24 myRadio (8, 9);
byte addresses[][6] = {"0"};

//Define Payload structure - in this case it's just 2 joysticks 
typedef     struct Buttons{
  bool A;
  bool B;
  bool X;
  bool Y;
}Buttons;

struct package_pin
{
int id = 2;
int xPosition1 ;
int yPosition1 ;
 //left joystick
int xPosition2 ;
int yPosition2 ;
Buttons buttons ;

_pin
};

//Instantiate package _pinobject and assign to varable data:
typedef struct package Package;
Package data;

//Joystick Setup 

const int VRx1 = A0;
const int VRy1 = A1;
const int VRx2 = A2;
const int VRy2 = A3 ; 

// Buttons 
const int A_Pin = 6 ;
int xPosition1 = 0 ;
int yPosition1 = 0 ;
int xPosition2 = 0 ;
int yPosition2 = 0;
s


//Mapped Joystick input variables 
int mapX1 = 0;
int mapY1 = 0;
int mapX2 = 0;
int mapY2 = 0;


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
    
    pinMode(VRx1, INPUT);
    pinMode(VRy1, INPUT);
    pinMode(VRx2, INPUT);
    pinMode(VRy2, INPUT);
    pinMode(A_Pin, INP_pinUT);
    

    Serial.begin(9600);
    delay(1000);

    // Set up the radio communication 
    myRadio.begin();  
    myRadio.setChannel(115); 
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.setDataRate( RF24_250KBPS ) ; 
    myRadio.openWritingPipe( addresses[0]);
    myRadio.stopListening();
    
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW); }

void loop(){

  //Left Joystick 
  xPosition1 = analogRead(VRx1);
  yPosition1 = analogRead(VRy1);
  //Right joystick
  xPosition2 = analogRead(VRx2);
  yPosition2 = analogRead(VRy2);
  
    mapX1 = map(xPosition1, 0, 1023, 0, 255);
    mapY1 = map(yPosition1, 0, 1023, 0, 255);    
  mapX2 = map(xPosition2, 0, 1023, 0, 255);
  mapY2 = map(yPosition2, 0, 1023, 0, 255); 

  data.id = 1;
  data.xPosition1 = mapX1;
  data.yPosition1 = mapY1;
  data.xPosition2 = mapX2;
  data.yPosition2 = mapY2;

  data.A_Button = (digitalRead(A_Pin) == HIGH);
  
  myRadio.write(&data, sizeof(data)); 
  // Check data is correct by printing to Serial Monitor. 
  Serial.print("\nPackage:");
  Serial.print(data.id);
  Serial.print("\n");
  Serial.println(data.xPosition1);
  Serial.println(data.yPosition1);
  Serial.println(data.A_Button);
  delay(50);
}
        
