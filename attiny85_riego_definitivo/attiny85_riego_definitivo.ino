/*
Irrigation software for atiny85
First version:10/2014
Author: Angel Gil
Feel free to use and modify this litle, litle, litle software

 Added code from (http://www.insidegadgets.com/2011/02/05/reduce-attiny-power-consumption-by-sleeping-with-the-watchdog-timer/).
 Thanks to:
 * KHM 2008 / Lab3/  Martin Nawrath nawrath@khm.de
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne
 *
 * Modified on 5 Feb 2011 by InsideGadgets (www.insidegadgets.com)
 * to suit the ATtiny85 and removed the cbi( MCUCR,SE ) section 
 * in setup() to match the Atmel datasheet recommendations
*/


#include <avr/sleep.h>
#include <avr/wdt.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif  

//****************************************************************

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;
  MCUSR &= ~(1<<WDRF);
  #if defined(__AVR_ATmega328P__)
   // start timed sequence
    WDTCSR|= (1<<WDCE) | (1<<WDE);
    // set new watchdog timeout value
    WDTCSR = bb;
    WDTCSR|= _BV(WDIE);
  #else //attiny85
    // start timed sequence
    WDTCR |= (1<<WDCE) | (1<<WDE);
    // set new watchdog timeout value
    WDTCR = bb;
    WDTCR |= _BV(WDIE);
  #endif
  
}
//****************************************************************  
// set system into the sleep state 
// system wakes up when wtchdog is timed out
//****************************************************************  
void system_sleep() {

  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System actually sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON


}
//****************************************************************  
// Watchdog Interrupt Service / is executed when  watchdog timed out

ISR(WDT_vect) {
  //I don't have anything to do here
#if defined(__AVR_ATmega328P__)//for testing purpose
   Serial.println("wake up"); 
#endif    
}
//****************************************************************  

/*CONSTANTES*/
//Pines
int pinCerrar_1 = 0;
int pinAbrir_1 = 1;
int pinHabilitarSensores=2;
int pinSensorLuz =A2;
int pinSensorHumedad =A3;
//unsigned long msdia = 86400000; //milisegundos que tiene un dia, auqnue el reloj no es preciso

int estado = 'A';
float luz;
float humedad;
int cDormido=0;
int nDormidoMediaHora=225;// 225 sueños son 8*225 = 1800s = media hora

//****************************************************************

void setup(){

#if defined(__AVR_ATmega328P__)//for testing purpose
  nDormidoMediaHora=3;//2 minutos modificamos el nmeroo de sueños si testeamos para que vaya ms rpido
  int pinCerrar_1 = 8;
  int pinAbrir_1 = 9;
  Serial.begin(9600);
#endif  

  Abrir();
  Cerrar();
  setup_watchdog(9);

}
//****************************************************************

//****************************************************************

//****************************************************************

void loop(){

#if defined(__AVR_ATmega328P__)//for testing purpose
   Serial.println("Testing"); 
   Serial.print("nDormidoMediaHora: "); 
   Serial.println(nDormidoMediaHora); 

   Serial.println(estado); 
   Serial.print("cDormido: "); 
   Serial.println(cDormido); 
   delay(1000);
 #endif 
  
  // set all used port to intput to save power
  pinMode(0,INPUT);
  pinMode(1,INPUT);
  pinMode(2,INPUT);  
  pinMode(3,INPUT);
  pinMode(4,INPUT);

  system_sleep();
  
  cDormido++;

  switch(estado) {
      case 'A':
          estadoA();
          break;
      case 'B':
          estadoB();
          break;
      case 'C':
          estadoC();
          break;
      case 'D':
          estadoD();
          break;
      default:
          estado = 'A';
  }    

}
//**************************************************************** 
void estadoA(){
  if(cDormido>=nDormidoMediaHora*2){
    cDormido=0;
    //Activa la lectura sensores
    //enabled sensor feed    
    pinMode(pinHabilitarSensores,OUTPUT);    
    digitalWrite(pinHabilitarSensores,HIGH);
    delay(1);
    //Dark?
    if(!dia()){
      estado ='B';
    }
    //Desactiva la lectura sensores
    //disable sensor feed
    digitalWrite(pinHabilitarSensores,LOW);
  }
}
//**************************************************************** 
void estadoB(){
  //Se duerme y despierta hasta pasado una hora
  //sleep and wake up until one hour
  if(cDormido>=nDormidoMediaHora * 2){
    cDormido=0;
    pinMode(pinHabilitarSensores,OUTPUT);    
    digitalWrite(pinHabilitarSensores,HIGH);
    delay(1);
    //Wet?
    if(humedo() ){
      estado ='D'; // pasa al estado de sueño profundo
    }else{
      estado ='C';//pasa al estado de riego
    }
    //Desactiva la lectura sensores
    //disable sensor feed
    digitalWrite(pinHabilitarSensores,LOW);
  }
}
//**************************************************************** 
void estadoC(){
  if(cDormido==1){
    Abrir();    
  }
  if(cDormido>=(nDormidoMediaHora/3)){//10 minutos regando
    cDormido=0;
    Cerrar();
    estado ='D';//pasa al estado de sueño profundo
  }

}
//**************************************************************** 
void estadoD(){
  if(cDormido>=(nDormidoMediaHora * 42)){//22 horas durmiendo y despertndose
    cDormido=0;
    estado ='A'; //pasa al estado de alerta
  }
}

//**************************************************************** 
//pulso de 30ms para abrir
void Abrir(){
  pinMode(pinAbrir_1,OUTPUT);
  digitalWrite(pinAbrir_1, HIGH);
  delay(30);
  digitalWrite(pinAbrir_1, LOW);
  #if defined(__AVR_ATmega328P__)//for testing purpose
    Serial.println("Riega");
  #endif  
}
//**************************************************************** 
//pulso de 30ms para cerrar
void Cerrar(){
  pinMode(pinCerrar_1,OUTPUT);
  digitalWrite(pinCerrar_1, HIGH);
  delay(30);
  digitalWrite(pinCerrar_1, LOW);
  #if defined(__AVR_ATmega328P__)//for testing purpose
    Serial.println("Cierrar riego");
  #endif  

}
//**************************************************************** 
boolean dia(){
  luz = analogRead(pinSensorLuz)/1024.0*100.0;

  if(luz<20.0){

    #if defined(__AVR_ATmega328P__)//for testing purpose
      Serial.println("Noche");
      Serial.println(luz);
    #endif

    return false;
  } 

  #if defined(__AVR_ATmega328P__)//for testing purpose
    Serial.println("Dia");
    Serial.println(luz);
  #endif

 return true; 
  
}
boolean humedo(){
  humedad = analogRead(pinSensorHumedad)/1024.0*100.0;

  if(humedad<60.0){

    #if defined(__AVR_ATmega328P__)//for testing purpose
      Serial.println("Seco");
      Serial.println(humedad);
    #endif
    
    return false;
  } 

  #if defined(__AVR_ATmega328P__)//for testing purpose
    Serial.println("Humedo");
  #endif

 return true; 
}
//**************************************************************** 






