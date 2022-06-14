#include <NintendoSwitchControlLibrary.h>
#include <SPI.h>  
#include "RF24.h" 
#include <EEPROM.h>
#include <DDCONlib.h>


// ==================== CONSTANT =======================
RF24 myRadio (9, 10);
const byte addresses[][6] = {"0"};  


//================== Timer Set up =============================

//int counter = 0;
//unsigned long val;
//int addr = 0;


//======================== Define Payload structure==========================

RightPackage dataR;
LeftPackage dataL;

//=================== Nintendo Switch Controls ===============================

typedef struct switch_command
{
 RightPackage dataR;
 LeftPackage dataL;
  
} switch_command;
switch_command current;

void tiltSticks(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry) {
    
    SwitchControlLibrary().moveLeftStick(lx, ly);
    SwitchControlLibrary().moveRightStick(rx, ry);
}


void activateButton(bool state, uint16_t button ){
  if (state == 1){
  SwitchControlLibrary().pressButton(button);
  }
  else if (state ==0) {
    SwitchControlLibrary().releaseButton(button);
  }
  
}

void activateHAT(bool state, uint8_t hat){
  if (state == 1){
  SwitchControlLibrary().pressHatButton(hat);
  }
  else if (state == 0) {
    SwitchControlLibrary().releaseHatButton(); // No input???
  }
}


//=============== Variables needed for operation =================

bool LFlag = 0;
bool RFlag = 0;
int received = 0;


//================================================================

void setup() 
{
 SwitchControlLibrary();
  
// Radio setup:
  myRadio.begin(); 
  myRadio.setChannel(0x34); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_2MBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop(){
  // Start timer to measure response time:
  
  while (myRadio.available())
    { 
      while(LFlag == 0){
        myRadio.read(&dataL, sizeof(dataL) );  
        if (dataL.id == 1){
          LFlag = 1;
          current.dataL = dataL;
          delay(5);
            
          }
    
    }
    while(RFlag == 0)
    {
      myRadio.read(&dataR, sizeof(dataR) );
      if (dataR.id == 2)
      {
            RFlag = 1;
            delay(5);
            current.dataR = dataR;
      }
    }
    
// SWITCH CONTROL
tiltSticks(current.dataL.joystick.xPos, current.dataL.joystick.yPos, current.dataR.joystick.xPos, current.dataR.joystick.yPos);

// This bit is poorly written, and super hardcoded
bool state;
uint16_t button;
uint8_t hat;

for (int button_index = 1; button_index <= 16; button_index++){
  switch(button_index){
    case 1: //A
    state = current.dataR.buttons.A;
    button = 0x0004; // Hexadecimal value corresponding to button A in SwitchControlLibrary
    activateButton(state, button);

    case 2: //B
    state = current.dataR.buttons.B;
    button = 0x0004; 
    activateButton(state, button);  
    case 3: //X
    state = current.dataR.buttons.X;
    button = 0x0008; 
    activateButton(state, button);  
    
    case 4: //Y
    state = current.dataR.buttons.Y;
    button = 0x0001; 
    activateButton(state, button);  
    
    case 5: //RJsb
    state = current.dataR.buttons.RJsb;
    button = 0x0800; 
    activateButton(state, button);  
    
    case 6: //Plus
    state = current.dataR.buttons.Plus;
    button = 0x0200;
    activateButton(state, button);  
    
    case 7: //RT
    state = current.dataR.buttons.RT;
    button = 0x0020; 
    activateButton(state, button);  
    
    case 8: //ZR
    state = current.dataR.buttons.ZR;
    button = 0x0080; 
    activateButton(state, button);  
    
    case 10: //ZL
    state = current.dataL.buttons.ZL;
    button = 0x0040;
    activateButton(state, button);  

    case 11: //Minus
    state = current.dataL.buttons.Minus;
    button = 0x0100;
    activateButton(state, button);

    case 12: //LJsb
    state = current.dataL.buttons.LJsb;
    button = 0x0100;
    activateButton(state, button);

    //For HAT buttons command is slightly different
    case 13: //UP
    state = current.dataL.buttons.UP;
    button = 0x00;
    activateHAT(state, button);

    case 14: //DOWN
    state = current.dataL.buttons.DOWN;
    button =  0x04;
    activateHAT(state, button);

    case 15: //LEFT
    state = current.dataL.buttons.LEFT;
    button = 0x06;
    activateHAT(state, button);

    case 16: //RIGHT
    state = current.dataL.buttons.RIGHT;
    button = 0x02;
    activateHAT(state, button);
  }
}

SwitchControlLibrary().sendReport();
    }

  while (!myRadio.available())
  {
  SwitchControlLibrary().sendReport();

  }

    LFlag = 0;      
    RFlag = 0;

}

  

//  unsigned long end_timer = millis();
//  counter = counter + 1;
//  
//  if ((counter % 5) == 0){
//    val = end_timer - start_timer;
//    EEPROM.put(addr, val);
//    
//    addr = addr + 1;
//
//    if (addr == EEPROM.length()) {
//    addr = 0;}