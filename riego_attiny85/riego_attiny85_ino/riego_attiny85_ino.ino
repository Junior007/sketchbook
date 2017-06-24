//****************************************************************

/*

 * Watchdog Sleep Example 
 
 * Demonstrate the Watchdog and Sleep Functions
 
 * Photoresistor on analog0 Piezo Speaker on pin 10
 
 * 
 
 
 
 * KHM 2008 / Lab3/  Martin Nawrath nawrath@khm.de
 
 * Kunsthochschule fuer Medien Koeln
 
 * Academy of Media Arts Cologne
 
 
 
 */

//****************************************************************



#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef cbi

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

#endif

#ifndef sbi

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#endif




int pinLed=0;

volatile boolean f_wdt=1;



void setup(){
  pinMode(pinLed,OUTPUT);
  setup_watchdog(9);

}





//****************************************************************

//****************************************************************

//****************************************************************

void loop(){
  //if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
  f_wdt=0;       // reset flag
  digitalWrite(pinLed,1);  // let led blink
  delay(2);               // wait until the last serial character is send
  pinMode(pinLed,INPUT); // set all used port to intput to save power
  system_sleep();
  pinMode(pinLed,OUTPUT); // set all ports into state before sleep

  //}

  digitalWrite(pinLed, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);               // wait for a second

}



//****************************************************************  
// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {

  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System actually sleeps here

    sleep_disable();                     // System continues execution here when watchdog timed out 

  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON

}



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

  // start timed sequence

  WDTCR |= (1<<WDCE) | (1<<WDE);

  // set new watchdog timeout value

  WDTCR = bb;

  WDTCR |= _BV(WDIE);





}

//****************************************************************  

// Watchdog Interrupt Service / is executed when  watchdog timed out

ISR(WDT_vect) {

  f_wdt=1;  // set global flag

}





