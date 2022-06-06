//
// Created by Nicolas Dehandschoewercker on 03/06/2022.
//
#include <DDCONlib.h>
#include <Arduino.h>

//SETUP LOOPS
void buttonSetup(){
    for (int thisPin=2; thisPin < 10;  thisPin++ ){
        pinMode(thisPin, INPUT_PULLUP);
    }
}


