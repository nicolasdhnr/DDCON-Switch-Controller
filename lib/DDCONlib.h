//
// Created by Nicolas Dehandschoewercker on 03/06/2022.
//

#ifndef DMD_NINTENDO_SWITCH_GAMING_DDCONLIB_H
#define DMD_NINTENDO_SWITCH_GAMING_DDCONLIB_H

#include <Arduino.h>
#include <RF24.h>


void buttonSetup();

//Define buttons
typedef struct RButtons {
    bool A = 0;
    bool B = 0;
    bool X = 0;
    bool Y = 0;
    bool Jsb = 0;
    bool Plus = 0;
    bool ZR = 0;
    bool RT = 0;
} RButtons;

typedef struct LButtons {
    bool UP = 0;
    bool RIGHT = 0;
    bool DOWN = 0;
    bool LEFT = 0;
    bool Jsb = 0;
    bool Minus = 0;
    bool ZL = 0;
    bool LT = 0;
} LButtons;

typedef struct Joystick {
    int xPos;
    int yPos;
} Joystick;


typedef struct RightPackage {
    int id = 1;
    Joystick joystick;
    RButtons buttons;
} RightPackage;

typedef struct LeftPackage {
    int id = 0;
    Joystick joystick;
    LButtons buttons;
} LeftPackage;



#endif //DMD_NINTENDO_SWITCH_GAMING_DDCONLIB_H
