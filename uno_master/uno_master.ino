
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

/* Define adresses of pipes that will be used to communicate with Nanos. They are defined as follows
00001 - Uno to Both Nanos as transmitter - requests a signal back. 
00002 - Left Nano to Uno
00003 - Right Nano to Uno
*/ 
const byte addresses[][6] = {"00001", "00002", "00003"}; 
int connectionLED = 5;

void setup() {
  pinMode(connectionLED, OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.openReadingPipe(2, addresses[2]; // 00003
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening()


}

void loop() {
  const int pollSignal [10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  radio.write(&pollSignal, sizeof(pollSignal));
  
  if (radio.available()) {
    
  }
  delay(10);

  
  
  
}
