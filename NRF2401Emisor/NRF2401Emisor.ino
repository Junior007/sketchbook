
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
* 
1*/

#include <SPI.h>
#include "RF24.h"
#include "Botones.h"

Boton boton_1;
Boton boton_2;
Boton boton_3;
Boton boton_4;
Boton boton_5;
Boton boton_6;

// set pin numbers:
//const int  pinComunicacionPerdida = 8;

int VELOCIDAD;
int CURVA;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;//=>transmitting

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 */
RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  //**RADIO
   ConfigurarRadio();
   digitalWrite(13, LOW);

   boton_2.set(5);
   boton_3.set(6);
   boton_4.set(7);
   boton_5.set(8);
   boton_6.set(A6);
}

void loop() {


  if(boton_1.buttonPush()){
    VELOCIDAD=0; 
    Serial.println("Push");
  }
  if(boton_2.buttonPush()){
    CURVA=0; 
    Serial.println("Push");
  }
  if(boton_3.buttonPush()){
    VELOCIDAD=100; 
    Serial.println("Push");
  }
  if(boton_4.buttonPush()){
    CURVA=100; 
    Serial.println("Push");
  }
  if(boton_5.buttonPush()){
    VELOCIDAD=-100; 
    Serial.println("Push");
  }
  if(boton_6.buttonPush()){
    CURVA=-100; 
    Serial.println("Push");
  }
  
  VELOCIDAD=-120; 
  CURVA=-100; 
  Comando(VELOCIDAD,CURVA);
  

  delay(1000);
  
} // Loop

//

void Comando(int VELOCIDAD, int CURVA){
  
    unsigned long for_send;
    for_send = VELOCIDAD>0?1100000000:1000000000;
    for_send+=abs(VELOCIDAD)*100000;
    for_send += CURVA>0?1000:0;
    for_send+=abs(CURVA);
  
    radio.stopListening();                                    // First, stop listening so we can talk.
    //Hacemos 5 intentos
    for(int i=0;i<5 && !radio.write( &for_send, sizeof(unsigned long) );i++){
      /*Serial.print(F("failed: "));
      Serial.println(i);*/
      //digitalWrite(pinComunicacionPerdida, HIGH);
    }
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not


    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }
    }
    
    if (!timeout){            // If waited longer than 200ms, indicate timeout and exit while loop
      unsigned long got_time;                                // Grab the response, compare, and send to debugging spew
      radio.read( &got_time, sizeof(unsigned long) );
      // Spew it
      Serial.print(F("Sent "));
      Serial.print(for_send);
      Serial.print(F(", Got response "));
      Serial.println(got_time);
    }      

   
    VELOCIDAD= recorta(for_send,100000,1000000000);
    CURVA = recorta(for_send,1,10000);
    Serial.println(for_send);
    Serial.println("");
    Serial.print(VELOCIDAD);
    Serial.print(" : ");
    Serial.print(CURVA);
    Serial.println("");
    Serial.println("");
}
//
int recorta(unsigned long comando, unsigned long ini, unsigned long fin){
  unsigned long final=comando;
  unsigned long inicio=comando;
  return ((inicio/ini)*ini-(final/fin)*fin)/ini;
}
//
void ConfigurarRadio(){
  //pinMode(pinComunicacionPerdida, OUTPUT);
  
  radio.begin();
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
}

