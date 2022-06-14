/* This code is to be used on the Arduino acting as Transmitter.
 *  Required hardware is 2 Joysticks.
 *  This intend for the RIGHT HAND DDCON
 *  
  */
 
#include "RF24.h"
#include <SPI.h>
#include <NintendoSwitchControlLibrary.h>

//Define buttons
typedef struct Buttons {
    bool A = 0;
    bool B = 0;
    bool X = 0;
    bool Y = 0;
    bool RJsb = 0;
    bool Plus = 0;
    bool ZR = 0;
    bool RT = 0;
} Buttons;

//typedef struct LButtons {
//    bool UP = 0;
//    bool RIGHT = 0;
//    bool DOWN = 0;
//    bool LEFT = 0;
//    bool LJsb = 0;
//    bool Minus = 0;
//    bool ZL = 0;
//    bool LT = 0;
//} LButtons;

typedef struct Joystick {
    int xPos;
    int yPos;
} Joystick;

typedef struct Package {
    int id = 0;
    Joystick joystick;
    Buttons buttons;
} RightPackage;

typedef struct switchCommand
{
    Package dataR;
    Package dataL;

} switchCommand;


void buttonSetup(){
    for (int thisPin=3; thisPin < 11;  thisPin++ ){
        pinMode(thisPin, INPUT_PULLUP);
    }
}

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
void printRightPackageInfo(Package data){
    Serial.print("\nPackage:");
    Serial.print(data.id);
    Serial.print("\n");
    Serial.print("Joystick xPos: ");
    Serial.println(data.joystick.xPos);
    Serial.print("Joystick yPos");
    Serial.println(data.joystick.yPos);

    Serial.print("A: ");
    Serial.println(data.buttons.A);
    Serial.print("B: ");
    Serial.println(data.buttons.B);
    Serial.print("X: ");
    Serial.println(data.buttons.X);
    Serial.print("Y: ");
    Serial.println(data.buttons.Y);
    Serial.print("RT: ");
    Serial.println(data.buttons.RT);
    Serial.print("ZR: ");
    Serial.println(data.buttons.A);
    Serial.print("Right Joystick button: ");
    Serial.println(data.buttons.RJsb);
    Serial.print("PLUS: ");
    Serial.println(data.buttons.Plus);
}

void iterateButtons(switchCommand current){
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
                state = current.dataL.buttons.ZR; //ZL
                button = 0x0040;
                activateButton(state, button);

            case 11: //Minus
                state = current.dataL.buttons.Plus; //Minus
                button = 0x0100;
                activateButton(state, button);

            case 12: //LJsb
                state = current.dataL.buttons.RJsb; //LJsb
                button = 0x0100;
                activateButton(state, button);

                //For HAT buttons command is slightly different
            case 13: //UP
                state = current.dataL.buttons.X; //Up
                button = 0x00;
                activateHAT(state, button);

            case 14: //DOWN
                state = current.dataL.buttons.B; //Down
                button =  0x04;
                activateHAT(state, button);

            case 15: //LEFT
                state = current.dataL.buttons.Y; // Left
                button = 0x06;
                activateHAT(state, button);

            case 16: //RIGHT
                state = current.dataL.buttons.A; // Right
                button = 0x02;
                activateHAT(state, button);
        }
    }
}

Buttons readPins(){
    Buttons buttons;
    for (int thisPin=2; thisPin < 10;  thisPin++ ){
        switch(thisPin){
            case 3:
                buttons.A = !digitalRead(thisPin);
                break;

            case 4:
                buttons.B = !digitalRead(thisPin);
                break;

            case 5:
                buttons.X = !digitalRead(thisPin);
                break;

            case 6:
                buttons.Y = !digitalRead(thisPin);
                break;

            case 7:
                buttons.RJsb = !digitalRead(thisPin);
                break;

            case 8:
                buttons.Plus = !digitalRead(thisPin);
                break;

            case 9:
                buttons.ZR = !digitalRead(thisPin);
                break;

            case 10:
                buttons.RT = !digitalRead(thisPin);

                break;

            default:
                Serial.println("ERROR");
        }
    }
    return buttons;
}


//Define parameters needed for radio
RF24 myRadio (11, 12);
byte addresses[][6] = {"0"};

  //==================Define Payload structure==========================
  
 Package data;
 Joystick joystick;
 Buttons button;

// ================= Set up components =================
//Joystick Setup 

const int VRx = A0;
const int VRy = A1;

//============ Radio Setup ===========
const uint8_t rDelay = 7; //this is based on 250us increments, 0 is 250us so 7 is 2 ms
const uint8_t rNum = 5; //number of retries that will be attempted 

void radioSetup(){
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
    
    //Set pins for joystick to input 
    pinMode(VRx, INPUT);
    pinMode(VRy, INPUT);

    //Set pins 3 to 4 to INPUT_PULLUP.
    buttonSetup();
    
    Serial.begin(9600);
    delay(1000);

    //set up a seed for the random delay.
    randomSeed(analogRead(0));

    // Set up the radio communication 
    radioSetup();

   if (!myRadio.begin()){
    Serial.println("radio hardware malfunction");
    while(true){
          Serial.println("radio hardware malfunction");

    }
  }
  
  else{
    Serial.println("Bootup Successful");
  }
}


void loop(){
  delay(50);
  //Left Joystick 
  joystick.xPos = map(analogRead(VRx), 0, 1023, 0, 255);
  joystick.yPos = map(analogRead(VRy), 0, 1023, 0, 255);

  //read all Buttons
  button = readPins();
  data.id = 2;
  data.joystick = joystick;
  data.buttons = button;

  

  printRightPackageInfo(data);
  
if (!myRadio.write(&data, sizeof(data))){  //send data and remember it will retry if it fails
    delay(random(5, 20)); //as anothxer back up, delay for a random amount of time and try again
    if (!myRadio.write(&data, sizeof(data))){
      Serial.println("Transmission failed");
      delay(random(5,20));
      
      }  
     }
     // Check data is correct by printing to Serial Monitor. 
 // Check data is correct by printing to Serial Monitor. 

  
}
