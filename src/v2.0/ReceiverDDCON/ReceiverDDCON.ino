#include <NintendoSwitchControlLibrary.h>
#include <SPI.h>  
#include "RF24.h" 
// #include <EEPROM.h>  
#include <DDCONlib.h>

RF24 myRadio (11, 12);
const byte addresses[][6] = {"0"};  

////================== Timer Set up =============================
//int counter = 0;
//unsigned long val;
//int addr = 0;

//==================Define Payload structure==========================

Joystick joystick;
Package data;

//=================== Nintendo Switch Controls ===============================
      
switchCommand current;

//=============== Variables needed for operation =================

bool LFlag = 0;
bool RFlag = 0;

//================================================================

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
 Serial.begin(115200);
 pushButton(Button::A, 500, 5); 
 SwitchControlLibrary();
  
// Radio setup:
  myRadio.begin(); 
  myRadio.setChannel(125); // Channel was chosen based on experiments. 
  myRadio.setPALevel(RF24_PA_LOW);
  myRadio.setDataRate( RF24_2MBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.setPayloadSize(sizeof(data));
  myRadio.startListening();
}

void loop()  {
  
// Start timer to measure response time:
//unsigned long start_timer = millis();


  // ============================= RADIO BIT (DO NOT TOUCH) =============================

  //Received Left DDCON Data
  while (myRadio.available())
    {
    while (!(LFlag && RFlag) == 1){
    myRadio.read( &data, sizeof(data) );  
    printRightPackageInfo(data);     
                                
    if (data.id == 2){
      LFlag = 1;
      current.dataL = data;
      delay(5);
      myRadio.read( &data, sizeof(data));
    }
    
    // Receive Right DDCON Data
    if (data.id == 1){
      RFlag = 1;
      delay(5);
      current.dataR = data;
    }
    
  iterateButtons(current); // Update the report to be sent to the Switch with Joystick states
  //Update 
  tiltSticks(current.dataL.joystick.xPos, current.dataL.joystick.yPos, current.dataR.joystick.xPos, current.dataR.joystick.yPos);
  SwitchControlLibrary().sendReport();
    }
    LFlag = 0;      
    RFlag = 0;
    }

  while (!myRadio.available())
  {
  SwitchControlLibrary().sendReport();

  }
  delay(5);

// ==== Code snippet used if response time measurement is required =========

//  unsigned long end_timer = millis();
//  counter = counter + 1;
//  
//  if ((counter % 50) == 0){
//    val = end_timer - start_timer;
//    EEPROM.put(addr, val);
//    
//    addr = addr + 1;
//
//    if (addr == EEPROM.length()) {
//    addr = 0;
//
//
//  }

  
}
