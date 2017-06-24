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
int led = 13;
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
   Serial.print("cDormido: "); 
   Serial.println(cDormido); 
   delay(1000);
 #endif 
  
  // set all used port to intput to save power
  pinMode(0,INPUT);
  pinMode(1,INPUT);
  pinMode(2,INPUT);  //Este no por que es el que habilita los sensores
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  
  
  system_sleep();
  
  cDormido++;
  if((cDormido%nDormidoMediaHora)==0){
    pinMode(led, OUTPUT);     
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    pinMode(led, INPUT);
  }

  

}


