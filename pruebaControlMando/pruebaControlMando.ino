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

unsigned int ms=0;
unsigned int incremento1cada5ms;


boolean isActing = false; //If the robot is executing command from remote
long timer;
const long TIME_OUT = 150;
float VELOCIDAD = 0;
float     CURVA = 0;
decode_results results;
bool recibiendo=false;
void setup()
{
  Serial.begin(250000);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if(abs(millis()-ms)>5){
    ms=millis();
    incremento1cada5ms++;
  }

 //If the robot does not receive any command, stop it
 if (isActing && (millis() - timer >= TIME_OUT)) {
    changeAction(0,0);
    isActing = false;
 }
 

   /* 
  if(incremento1cada5ms%20==0){
    direccion();
    motor(); 
  }*/
  recibeComando();
  //
  if(incremento1cada5ms%10==0){
    Serial.println(CURVA);
  /*    
    Serial.print(VIzq());
    Serial.print("-");
    Serial.println(VDer());
    
   
   Serial.print(pwm_der);//, 4);
   Serial.print("-");//, 4);
   Serial.println(pwm_izq);//, 4) ;*/
  }
}

//
void recibeComando(){
  if (irrecv.decode(&results)) {
    unsigned long result =results.value;
    irrecv.resume(); // Receive the next value
    processResult(result);
  }
}
//mandamos la instrucción al rover
void processResult(unsigned long res) {
  
  if(IR_CODE_TURN_LEFT ==res || IR_CODE_TURN_LEFT_1==res)
    changeAction(VELOCIDAD, -0.5); //Turn the robot left
  else if(IR_CODE_TURN_RIGHT ==res || IR_CODE_TURN_RIGHT_1==res)
    changeAction(VELOCIDAD, 0.5); //Turn the robot left
  else if(IR_CODE_FORWARD_C ==res || IR_CODE_FORWARD_C_1==res)
    changeAction(50, CURVA); //Turn the robot left
  else if(IR_CODE_BACKWARD_C ==res || IR_CODE_BACKWARD_C_1==res)
    changeAction(-50, CURVA); //Turn the robot left
}
void changeAction(float VEL, float CUR) {
  VELOCIDAD=VEL;
  CURVA=CUR;

  timer = millis();
  isActing = true;
}
//**********************************************
