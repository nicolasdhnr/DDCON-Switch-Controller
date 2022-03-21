#include <DDCONCustomLibrary.h>
#include <SwitchControlLibrary.h>


// Define Buttons enum - These Buttons are present on only one controller
s


typedef  struct LeftButtons{
  bool UP;
  bool DOWN;
  bool LEFT;
  bool RIGHT;
} LeftButtons;

typedef struct RightButtons{
  bool A;
  bool B;
  bool LEFT;
  bool RIGHT;
} RightButtons;

//Define struct for Joysticks
typedef struct Joystick{
int xPos;
int yPos ;
} Joystick;

//
typedef struct PackageRight{
int id;
Joystick joystick;
RightButtons buttons;
} PackageRight;

typedef struct PackageLeft{
int id;
Joystick joystick;
LeftButtons buttons;
} PackageLeft;


//Define a report structure
//TODO: Add support for all 16 buttons.

typedef enum switchReport{
PackageLeft leftController;
PackageRight rightController;
};


/*
Takes Josytick struct as input and formats it in a way that it can be sent to the switch.
@Joystick
*/
void mapJoystickInputs(Joystick joystick){
    Joystick.xPos = map(xPos, 0, 1023, 0, 255);
    Joystick.yPos = map(yPos, 0, 1023, 0, 255);  
}
void pressButton(bool state, string Button)
{
  if (state == 1){
  SwitchControlLibrary().pressButton(Button::Button);
  }
  else if (state ==0) {
  SwitchControlLibrary().releaseButton(Button::Button);
  }
  
}

void pressHAT(bool state,  hat)
{
  if (state == 1){
    // yeah idk if this is gonna work but coolio
  SwitchControlLibrary().pressHatButton(HAT::hat);
  }
  else if (state == 0){
  SwitchControlLibrary().releaseHatButton();
  }
}

void tiltSticks(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry) {
    SwitchControlLibrary().moveLeftStick(lx, ly);
    SwitchControlLibrary().moveRightStick(rx, ry);
}

void sendReport(switchReport report)
{
  pressButton(report.leftController.buttons.A, "A");
  pressButton(report.leftController.buttons.B, "B");
  pressButton(report.leftController.buttons.X, "X");
  pressButton(report.leftController.buttons.Y, "Y");

  pressHAT(report.rightController.buttons.UP, "UP");
  pressHAT(report.rightController.buttons.DOWN, "DOWN");
  pressHAT(report.rightController.buttons.LEFT, "LEFT");
  pressHAT(report.rightController.buttons.UP, "RIGHT");

  tiltSticks(report.rightController.joystick.xPos,eport.rightController.joystick.xPos,)


}

