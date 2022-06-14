// This library is designed to help control the Duchenne Dual Con, a custom Nintendo Switch Control designed by
// Imperial College London Bioengineers.
// Created by Nicolas Dehandschoewercker on 03/06/2022.
// Property of DAPP2 Group 3
// Imperial College London
//


#ifndef DMD_NINTENDO_SWITCH_GAMING_DDCONLIB_H
#define DMD_NINTENDO_SWITCH_GAMING_DDCONLIB_H

#include <Arduino.h>
#include <NintendoSwitchControlLibrary.h>
#include "RF24.h"
#include <SPI.h>

void rightButtonSetup();
void leftButtonSetup();

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


typedef struct Joystick {
    int xPos;
    int yPos;
} Joystick;

typedef struct Package {
    int id = 0;
    Joystick joystick;
    Buttons buttons;
} RightPackage;


// FOR RECEIVER:
void activateHAT(bool state, uint8_t hat);
void activateButton(bool state, uint16_t button );
void tiltSticks(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry);

typedef struct switchCommand
{
    Package dataR;
    Package dataL;

} switchCommand;

void iterateButtons(switchCommand current);
Buttons readRightPins();
Buttons readLeftPins();

// Tool Functions
void printRightPackageInfo(Package dataR);

#endif //DMD_NINTENDO_SWITCH_GAMING_DDCONLIB_H
