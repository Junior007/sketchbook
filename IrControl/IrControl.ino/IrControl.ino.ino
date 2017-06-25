/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
//https://learn.sparkfun.com/tutorials/ir-communication
int RECV_PIN = 52;
unsigned long res[100];
int count =0;
unsigned long mu =millis();
IRrecv irrecv(RECV_PIN);

//mando gyr, canal C
#define IR_CODE_TURN_LEFT 0x8C4BA750
#define IR_CODE_TURN_LEFT_1 0x23E38F69

#define IR_CODE_TURN_RIGHT   0xA90A0674  //Usa dos códigos, tiene dos modos en el canal c
#define IR_CODE_TURN_RIGHT_1 0xB7B891DB

#define IR_CODE_FORWARD_C   0x932768D1
#define IR_CODE_FORWARD_C_1 0xFB8F80B8

#define IR_CODE_BACKWARD_C   0x739780D9
#define IR_CODE_BACKWARD_C_1 0x8ED1BF61


 //x veces
decode_results results;
bool recibiendo=false;
void setup()
{
  Serial.begin(250000);
  Serial.println("hola");
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long result =results.value;
    irrecv.resume(); // Receive the next value
    recibiendo=true;
    res[count]=result;
    count++;
    mu=millis();
  }
  
  if(millis()-mu>300 && recibiendo){
    recibiendo=false;
    Serial.print("Serie datos: ");
    Serial.println(count);
    for(int i=0;i<count;i++){
      if(IR_CODE_TURN_LEFT ==res[i] || IR_CODE_TURN_LEFT_1==res[i])
        Serial.println("            giro izquierda");
      if(IR_CODE_TURN_RIGHT ==res[i] || IR_CODE_TURN_RIGHT_1==res[i])
        Serial.println("            giro derecha");
      if(IR_CODE_FORWARD_C ==res[i] || IR_CODE_FORWARD_C_1==res[i])
        Serial.println("            A TOPE ADELANTE");
      if(IR_CODE_BACKWARD_C ==res[i] || IR_CODE_BACKWARD_C_1==res[i])
        Serial.println("            A TOPE ATRÁS");        
        
      Serial.println( res[i],HEX);
    }
    count=0;
   /*
    Serial.print("---");
    Serial.println(res[0], HEX);
    count=0;*/
  }
}
