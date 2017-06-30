/*
Irrigation software for atmega328
First version:07/2017
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

const int temp_p = A4;
const int luz_p = A5;
const int lectura_p = 2;


//****************

#include <avr/sleep.h>
#include <avr/wdt.h>


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
//*************************************************************

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  analogReference(INTERNAL);//ref 1.1 volt
  //sensores
  pinMode(lectura_p, OUTPUT);
  setup_watchdog(9);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(2,HIGH); //cierra lectura
  system_sleep();
  digitalWrite(2,LOW); //abre lectura
  delay(10);
  Serial.println(temp());

  Serial.println(luz());
  
  Serial.println("OK");
  
  delay(5000);        // delay in between reads for stability

  while (Serial.available() > 0) {
    if(Serial.read()=='1'){
      digitalWrite(lectura_p,HIGH);  
    }else if(Serial.read()=='0'){
      digitalWrite(lectura_p,LOW);
    }
    Serial.print(Serial.read());

  }

  
}

//
float temp(){
  //valor = 0    => 0V   => 2ºC
  //valor = 1023 => 1.1V => 2+(1.1/0.01) ºC = 112 º C. Por encima ya no podemos leer con esta configuracion
  //valor = ???? => 1.48V =>150ºC
  // T  = 2 + (1.1 x valor)/(1023 * 0.01) 
  float valor = analogRead(temp_p);
  float temp = 2.0 + (1.1 * valor)/(1023.0 * 0.01) ;
  return temp;
}
//
int luz(){
  int valor = analogRead(luz_p);
  return valor;
}




