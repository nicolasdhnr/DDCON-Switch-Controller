
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
int connectionLED = 3;
String left_key []= {"X - L Stick", "Y - L Stick","L Button","- Button","UP","RIGHT","LEFT","DOWN","CAPTURE", "L RAIL"};
//"X - R Stick","Y - R Stick","R Button","+ Button",, "X","A","Y","B","HOME", "L RAIL"//
int L_pins [] = {A0,A1,6,7,8,9,10,11,12,13};

void setup() {
  pinMode(connectionLED, OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MAX);

  for (int i = 0; i < 10; i++) {
    pinMode(L_pins[i], INPUT);
  }
}

void loop() {
  digitalWrite(connectionLED,LOW);
  while (radio.available()){
    digitalWrite(connectionLED,HIGH);
    radio.startListening();
    int left_value [10] = {0};
    radio.read(&left_value, sizeof(left_value));
    radio.stopListening();
    for (int i = 0; i < 10; i++) {
      if (i<2){
        int stick_value = analogRead(L_pins[i]);
        left_value[i] = map(stick_value, 0, 1023, -512, 512);
      }
      else {
        boolean button_value = digitalRead(L_pins[i]);
        left_value[i] = button_value;
      }
  } 
    radio.write(&left_value, sizeof(left_value));
    delay(5);
  }
  
}
