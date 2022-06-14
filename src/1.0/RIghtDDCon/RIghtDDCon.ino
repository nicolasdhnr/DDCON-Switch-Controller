/* This code is to be used on the Arduino acting as Transmitter.
 *  Required hardware is 2 Joysticks.
 */

#include <SPI.h>
#include <DDCONlib.h>
#include "RF24.h"
#include "printf.h"

//Define parameters needed for radio
RF24 myRadio (10, 11);
uint8_t address[][6] = {"1Node", "2Node"};

//==================Define Buttons and Payload Structure=========================

RightPackage data;

// ================= Set up components =================
//Joystick Setup 
const int VRx = A0;
const int VRy = A1;

//============ Radio Setup ===========
const uint8_t rDelay = 7; //this is based on 250us increments, 0 is 250us so 7 is 2 ms
const uint8_t rNum = 5; //number of retries that will be attempted 
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = false;  // true = TX role, false = RX role

//void controllerRadioSetup(){
//    myRadio.begin();
//    myRadio.setChannel(108);
//    myRadio.setRetries(rDelay,rNum);
//    myRadio.setPALevel(RF24_PA_MAX);
//    myRadio.setDataRate( RF24_2MBPS ) ;
//    myRadio.openWritingPipe(addresses[0]);
//    myRadio.setPayloadSize(sizeof(data));
//    myRadio.stopListening();
//}
    

void setup(){
    Serial.println("Starting up ..");
    delay(1000);
    
    //Set pins for joystick to input  
    pinMode(VRx, INPUT);
    pinMode(VRy, INPUT);

    //Set pins for buttons to input
    buttonSetup();


    delay(1000);
    randomSeed(analogRead(5));

    // Set up the radio communication 
    //controllerRadioSetup();
    Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!myRadio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  // To set the radioNumber via the Serial monitor on startup
  Serial.println(F("Which radio is this? Enter '0' or '1'. Defaults to '0'"));
  while (!Serial.available()) {
    // wait for user input
  }
  char input = Serial.parseInt();
  radioNumber = input == 1;
  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  myRadio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  myRadio.setPayloadSize(sizeof(data)); // float datatype occupies 4 bytes

  // set the TX address of the RX node into the TX pipe
  myRadio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
  myRadio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

  // additional setup specific to the node's role
  if (role) {
    myRadio.stopListening();  // put radio in TX mode
  } else {
    myRadio.startListening(); // put radio in RX mode
  }

  // For debugging info
  // printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data

    //Chedck if radio has turned on properly 
    if (!myRadio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }
}

void loop(){
  delay(50);
  
//Read current state of joystick
  data.joystick.xPos = map(analogRead(VRx), 0, 1023, 0, 255);
  data.joystick.yPos = map(analogRead(VRy), 0, 1023, 0, 255);

  readRightPins(data);

Serial.println(sizeof(data));
if (role) {
    // This device is a TX node
    unsigned long start_timer = micros();                    // start the timer
    bool report = myRadio.write(&data, sizeof(float));      // transmit & save the report
    unsigned long end_timer = micros();                      // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
    } else {
       delay(random(5, 20)); //as another back up, delay for a random amount of time and try again
    if (!myRadio.write(&data, sizeof(data)))
      {
        Serial.println("Transmission not successful");
      delay(random(5,20));
      }
    }

    // to make this example readable in the serial monitor
    delay(1000);  // slow transmissions down by 1 second

  } else {
    // This device is a RX node

    uint8_t pipe;
    if (myRadio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = myRadio.getPayloadSize(); // get the size of the payload
      myRadio.read(&data, bytes);            // fetch payload from FIFO
      Serial.print(F("Received "));
      Serial.print(bytes);                    // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);                     // print the pipe number
      Serial.print(F(": "));
             
    }
  } // role

  if (Serial.available()) {
    // change the role via the serial monitor

    char c = toupper(Serial.read());
    if (c == 'T' && !role) {
      // Become the TX node

      role = true;
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      myRadio.stopListening();

    } else if (c == 'R' && role) {
      // Become the RX node

      role = false;
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      myRadio.startListening();
    }
  }

  
//if (!myRadio.write(&data, sizeof(data)))
//  {  //send data and remember it will retry if it fails
//    delay(random(5, 20)); //as another back up, delay for a random amount of time and try again
//    if (!myRadio.write(&data, sizeof(data)))
//      {
//        Serial.println("Transmission not successful");
//      delay(random(5,20));
//      
//      }   
//  }

}
       
