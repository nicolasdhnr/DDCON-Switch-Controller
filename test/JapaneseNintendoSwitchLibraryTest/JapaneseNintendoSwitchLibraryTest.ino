// ライブラリを読み込むためのコード
#include <NintendoSwitchControlLibrary.h>
#define delayTime 20 

const int VRx1 = A0;
const int VRy1 = A1;
const int VRx2 = A2;
const int VRy2 = A3 ; 

int xPosition1 = 0 ;
int yPosition1 = 0 ;
int xPosition2 = 0 ;
int yPosition2 = 0;

int mapX1 = 0;
int mapY1 = 0;
int mapX2 = 0;
int mapY2 = 0;

// マイコンのセット時に1度だけ行われる処理
void setup(){
    // Switchがマイコンを認識するまでは信号を受け付けないため、適当な処理をさせておく
    pushButton(Button::A,500, 5);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(VRx1, INPUT);
    pinMode(VRy1, INPUT);
    pinMode(VRx2, INPUT);
    pinMode(VRy2, INPUT);
    
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW); 

}

// ここに記述した内容がループされ続ける
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

  tiltLeftAndRightStick(mapX1, mapY1, mapX2, mapY2, 500);
}
