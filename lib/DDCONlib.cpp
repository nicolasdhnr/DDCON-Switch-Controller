// This library is designed to help control the Duchenne Dual Con, a custom Nintendo Switch Control designed by
// Imperial College London Bioengineers.
// Created by Nicolas Dehandschoewercker on 03/06/2022.
// Property of DAPP2 Group 3
// Imperial College London
//


#include <DDCONlib.h>

//SETUP LOOPS

/* Function: rightButtonSetup()
 * ------------------------
 * Sets all button pins to INPUT_PULLUP.
 * returns: void
 */
void rightButtonSetup(){
    for (int thisPin=5; thisPin < 9;  thisPin++ ) {
        pinMode(thisPin, INPUT_PULLUP);
    }
    pinMode(13, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP); //ZR
    pinMode(A3, INPUT_PULLUP);
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);

}

/* Function: leftButtonSetup()
 * ------------------------
 * Sets all button pins to INPUT_PULLUP.
 * returns: void
 */
//#TODO:
// - Add mapping guide
void leftButtonSetup(){
    for (int thisPin=3; thisPin < 8;  thisPin++ ){
        pinMode(thisPin, INPUT_PULLUP);
    }
    pinMode(11, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP); //ZL
    pinMode(A3, INPUT_PULLUP); //ZT
}
/* Function: tiltStick(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry)
 * ------------------------
 * Calls SwitchControlLibrary() functions to update the joystick positions.
 * lx: Left joystick x position
 * ly: Left joystick y position
 * rx: Right joystick x position
 * ry: Right joystick y position
 * returns: void
 */
void tiltSticks(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry) {

    SwitchControlLibrary().moveLeftStick(lx, ly);
    SwitchControlLibrary().moveRightStick(rx, ry);
}

/* Function:  activateButton(bool state, uint16_t button )
 * ------------------------
 * Sets all button pins to INPUT_PULLUP.
 *
 * returns: void
 */
void activateButton(bool state, uint16_t button ){
    if (state == 1){
        SwitchControlLibrary().pressButton(button);
    }
    else if (state ==0) {
        SwitchControlLibrary().releaseButton(button);
    }

}

/* Function: activateHAT (bool state, uint8_t hat)
 * ------------------------
 * Activates or deactivates a HAT button based on its state.
 * state: Boolean representing the state of the HAT button to be updated.
 * hat: the integer value corresponding to the HAT in the SwitchControlLibrary
 * returns: void
 */
void activateHAT(bool state, uint8_t hat){
    if (state == 1){
        SwitchControlLibrary().pressHatButton(hat);
    }
    else if (state == 0) {
        SwitchControlLibrary().releaseHatButton(); // No input???
    }
}

/* Function: printRightPackageInfo (Package data)
 * ------------------------|
 * Prints all information relevant to the package being sent.
 */
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
    Serial.println(data.buttons.ZR);
    Serial.print("Right Joystick button: ");
    Serial.println(data.buttons.RJsb);
    Serial.print("PLUS: ");
    Serial.println(data.buttons.Plus);
}
/* iterateButtons(switchCommand current)
 * ------------------------|
 * Maps each button to its corresponding 8 bit integer in the SwitchControlLibrary.
 * Then, the button states are passed to the activateButton function.
 * For
 * current: switchCommand struct containing the states of the Left and Right Data Combined.
 *
 */
void iterateButtons(switchCommand current){
    bool state;
    uint16_t button;
    uint8_t hat;

    for (int button_index = 1; button_index <= 16; button_index++){
        switch(button_index){
            case 1: //A
                button = 0x0004; // Hexadecimal value corresponding to button A in SwitchControlLibrary
                activateButton(current.dataR.buttons.A, button);

            case 2: //B
                button = 0x0004;
                activateButton(current.dataR.buttons.B, button);

            case 3: //X
                button = 0x0008;
                activateButton(current.dataR.buttons.X, button);

            case 4: //Y
                button = 0x0001;
                activateButton(current.dataR.buttons.Y, button);

            case 5: //RJsb
                button = 0x0800;
                activateButton(current.dataR.buttons.RJsb, button);

            case 6: //Plus
                button = 0x0200;
                activateButton(current.dataR.buttons.Plus, button);

            case 7: //RT
                button = 0x0020;
                activateButton(current.dataR.buttons.RT, button);

            case 8: //ZR
                button = 0x0080;
                activateButton(current.dataR.buttons.ZR, button);

                // We now need to handle the left-hand buttons.
            case 10: //ZL
                button = 0x0040;
                activateButton(current.dataL.buttons.ZR, button);

            case 11: //Minus
                button = 0x0100;
                activateButton(current.dataL.buttons.Plus, button);

            case 12: //LJsb
                button = 0x0100;
                activateButton(current.dataL.buttons.RJsb, button);

                //For HAT buttons command is slightly different
            case 13: //UP
                button = 0x00;
                activateHAT(current.dataL.buttons.B, button);

            case 14: //DOWN
                button =  0x04;
                activateHAT(current.dataL.buttons.Y, button);

            case 15: //LEFT
                button = 0x06;
                activateHAT(current.dataL.buttons.X, button);

            case 16: //RIGHT
                button = 0x02;
                activateHAT(current.dataL.buttons.A, button);
        }
    }
}

/* Buttons readPins()
 * ------------------------|
 * Reads each button pin and assigns it state to a location in the button struct
 * returns: Button struct with updated button state values.
 */
Buttons readLeftPins(){
    Buttons buttons;
    for (int thisPin=3; thisPin < 8;  thisPin++ ){
        switch(thisPin){
            case 3:
                buttons.A = !digitalRead(thisPin);
                break;

            case 4:
                buttons.B = !digitalRead(thisPin);
                break;

            case 5:
                buttons.Y = !digitalRead(thisPin);
                break;

            case 6:
                buttons.Plus = !digitalRead(thisPin);
                break;

            case 7:
                buttons.RJsb = !digitalRead(thisPin);
                break;
        }
        // TODO: Ensure this bit works
        // We want to give the option for the user to use the normal joystick button if they feel comfortable to do so.
        // Either button will work.
        buttons.RJsb |= (!digitalRead(13));
        buttons.X = !digitalRead(11);
        buttons.ZR = !digitalRead(A2);
        buttons.RT = !digitalRead(A3);

    }
    return buttons;
}

Buttons readRightPins(){
    Buttons buttons;
    for (int thisPin=5; thisPin < 9;  thisPin++ ){
        switch(thisPin){

            case 5:
                buttons.Y = !digitalRead(thisPin);
                break;

            case 6:
                buttons.X = !digitalRead(thisPin);
                break;

            case 7:
                buttons.Plus = !digitalRead(thisPin);
                break;

            case 8:
                buttons.RJsb = !digitalRead(thisPin);
        }
        // TODO: Ensure this bit works
        // We want to give the option for the user to use the normal joystick button if they feel comfortable to do so.
        // Either button will work.
        buttons.RJsb |= (!digitalRead(13));

        buttons.ZR = !digitalRead(A2);
        buttons.RT = !digitalRead(A3);
        buttons.A = !digitalRead(A4);
        buttons.B = !digitalRead(A5);

    }
    return buttons;
}


