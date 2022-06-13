#include <NintendoSwitchControlLibrary.h>
#include <SPI.h>  
#include "RF24.h" 
#include <EEPROM.h>

RF24 myRadio (9, 10);
const byte addresses[][6] = {"0"};  

//================== Timer Set up =============================
int counter = 0;
unsigned long val;
int addr = 0;




//==================Define Payload structure==========================
typedef struct Buttons{
  bool A = 0;
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
int id = 0;
Joystick joystick;
Buttons button;    
} Package;

Package data;



//=================== Nintendo Switch Controls ===============================
typedef struct switch_command
{
 Package dataL;
 Package dataR;
  
} switch_command;
      
    
switch_command current;

void tiltSticks(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry) {
    
    SwitchControlLibrary().moveLeftStick(lx, ly);
    SwitchControlLibrary().moveRightStick(rx, ry);
}

void ButtonState(bool state){
  if (state == 1){
  SwitchControlLibrary().pressButton(Button::A);
  }
  else if (state ==0) {
    SwitchControlLibrary().releaseButton(Button::A);
  }
  
}

//=============== Variables needed for operation =================

bool LFlag = 0;
bool RFlag = 0;
int received = 0;

//Tool Functions
void printPackageInfo(Package data){
  Serial.print("\nPackage:");
  Serial.print(data.id);
  Serial.print("\n");
  Serial.println(data.joystick.xPos);
  Serial.println(data.joystick.yPos);
}

//================================================================

void setup() 
{
 pushButton(Button::A, 500, 5); 
 SwitchControlLibrary();
  
// Radio setup:
  myRadio.begin(); 
  myRadio.setChannel(0x34); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_2MBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.setPayloadSize(sizeof(data));
  myRadio.startListening();
}

void loop()  {
  // Start timer to measure response time:
  unsigned long start_timer = millis();
  
  while (myRadio.available())
    {
    
    while (!(LFlag && RFlag) == 1){
    myRadio.read( &data, sizeof(data) );  
    printPackageInfo(data);                                 
    if (data.id == 1){
      LFlag = 1;
      current.dataL = data;
      delay(5);
      myRadio.read( &data, sizeof(data));
    }
    
    
    if (data.id == 2){
      RFlag = 1;
      delay(5);
      current.dataR = data;
    }

  tiltSticks(current.dataL.joystick.xPos, current.dataL.joystick.yPos, current.dataR.joystick.xPos, current.dataR.joystick.yPos);
//  ButtonState(current.A_Button);
  SwitchControlLibrary().sendReport();
    }
    LFlag = 0;      
    
    RFlag = 0;
    }
   // printPackageInfo(data);
    
  tiltSticks(current.dataL.joystick.xPos, current.dataL.joystick.yPos, current.dataR.joystick.xPos, current.dataR.joystick.yPos);
  pushButton(current.dataL.button.A);
//  ButtonState(current.A_Button);
  SwitchControlLibrary().sendReport();
  while (!myRadio.available())
  {
  tiltSticks(current.dataL.joystick.xPos, current.dataL.joystick.yPos, current.dataR.joystick.xPos, current.dataR.joystick.yPos);
  pushButton(current.dataL.button.A);
  SwitchControlLibrary().sendReport();

  }
  delay(5);

  unsigned long end_timer = millis();
  counter = counter + 1;
  
  if ((counter % 50) == 0){
    val = end_timer - start_timer; // Find the time elapsed during code execution
    EEPROM.put(addr, val); //Store the data into the EEPROM
    
    addr = addr + 1; // Move to the next memory address

    if (addr == EEPROM.length()) {
    addr = 0; // If the EEPROM is filled, return to the start of the of the code. 


  }

  }
}
