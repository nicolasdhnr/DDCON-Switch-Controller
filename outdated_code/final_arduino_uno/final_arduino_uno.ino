#include <SPI.h>  
#include "RF24.h" 

RF24 myRadio (7, 8); 

int led1 = 2;
int led2 = 3;

struct package
{
  int id = 2;
  bool back;
  bool top;
  bool menu;
  bool settings;
  bool Up;
  bool Left;
  bool Right;
  bool Down;
  int x;
  int y;
};


struct switch_command
{
  int id = 2;
  //left controller
  bool  ZL;
  bool  L;
  bool  Minus;
  bool  Screenshot;
  bool  Up;
  bool  Left;
  bool  Right;
  bool  Down;
  int   StickL_x;
  int   StickL_y;

  //right controller
  bool  ZR;
  bool  R;
  bool  Plus;
  bool  Home;
  bool  X;
  bool  Y;
  bool  A;
  bool  B;
  int   StickR_x;
  int   StickR_y;
};
struct switch_command current;

byte addresses[][6] = {"0"}; 

int received = 0;


typedef struct package Package;
Package data;
Package data1;
Package data2;



void setup() 
{
  Serial.begin(9600);
  delay(1000);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  
}


void loop()  
{
  delay(100);
  while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("\nPackage:");
    Serial.print(data.id);
    Serial.print("\n");
    Serial.println(data.x);
    Serial.println(data.y);
    
    if (data.id == 1){
      data1 = data;
      received += 1;
      current.ZL = data1.back;
      current.L = data1.top;
      current.Minus = data1.menu;
      current.Screenshot = data1.settings;
      current.Up = data1.Up;
      current.Left = data1.Left;
      current.Right = data1.Right;
      current.Down = data1.Down;
      current.StickL_x = data1.x;
      current.StickL_y = data1.y;

    }
    else if (data.id == 0){
      data2 = data;
      current.ZR = data2.back;
      current.R = data2.top;
      current.Plus = data2.menu;
      current.Home = data2.settings;
      current.X = data2.Up;
      current.Y = data2.Left;
      current.A = data2.Right;
      current.B = data2.Down;
      current.StickR_x = data2.x;
      current.StickR_y = data2.y;
      received += 1;
    }
    received = 0;
 }
    
