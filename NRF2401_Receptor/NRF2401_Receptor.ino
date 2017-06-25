
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
* 1 - GND   BLACK   - GND
* 2 - VCC   RED     - 3.3 Volts
* 3 - CE    ORANGE  -  9
* 4 - CSN   YELLOW  - 10
* 5 - SCK   GREEN   - 13
* 6 - MOSI  BLUE    - 11
* 7 - MISO  VIOLET  - 12
* 8 - IRQ   GRAY    - 
*/

#include <SPI.h>
#include "RF24.h"


/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;//=>receiving

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

int VELOCIDAD;
int CURVA;

void setup() {
  Serial.begin(115200);
  Serial.println("Hola");
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  //radio.setPALevel(RF24_PA_LOW);
  
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {

  unsigned long comando;
  
  if( radio.available()){
                                                                  // Variable for the received timestamp
    while (radio.available()) {                                   // While there is data ready
      radio.read( &comando, sizeof(unsigned long) );             // Get the payload
    }

    radio.stopListening();                                        // First, stop listening so we can talk   

    radio.write( &comando, sizeof(unsigned long) );              // Send the final one back.      
    radio.startListening();                                       // Now, resume listening so we catch the next packets.     
    
    VELOCIDAD= recorta(comando,100000,1000000000);
    CURVA = recorta(comando,1,10000);
    Serial.println("");
    Serial.print(VELOCIDAD);
    Serial.print(" : ");
    Serial.print(CURVA);
    Serial.println("");
    Serial.println("");

 }
} // Loop

int recorta(unsigned long comando, unsigned long ini, unsigned long fin){
  unsigned long final=comando;
  unsigned long inicio=comando;
  final = final - (final/fin)*fin;
  inicio = inicio - (inicio/ini)*ini;
  return (final-inicio)/ini;
}

