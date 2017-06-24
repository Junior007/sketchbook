
#include <avr/sleep.h>
//#include <avr/wdt.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>

#ifndef cbi

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

#endif

#ifndef sbi

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#endif



unsigned long msdia = 86400000; //milisegundos que tiene un dia
unsigned int dia;
unsigned int hora;
unsigned int minuto;
unsigned int segundo;

/*FIN DE CONSTANTES*/


//variables
unsigned long  tiempo = 0 ;
unsigned long  tiempo_dormido = 0 ;
unsigned long  tiempo_despierto = 0 ;
//constantes
const int min_humedad=10;
const int max_humedad=80;
const int limiteLuz=20; //20% de luz

//pines
const int pinAbrir_1 = 0;
const int pinCerrar_1 = 1;
const int pinHabilitarLecturas=2;

const int luzPin = A2;    // pin that the sensor is attached to
const int humedadPin = A3;    // pin that the sensor is attached to




long sensorLuz=0.00;
long sensorHumedad=0.00;

//****************************************************************

void setup(){

  setup_watchdog(9);

}
//****************************************************************

//****************************************************************

//****************************************************************

void loop(){

  /*
  tiempo = tiempo_dormido + millis();
  
  
  dia = int(tiempo / msdia);

  hora    = tiempo / (3600000);
  minuto  = tiempo / (60000);
  segundo = tiempo / (1000);

  segundo %= 60;
  minuto %= 60;
  hora %= 60;
  */
  /*
  pinMode(A1,INPUT); // set all used port to intput to save power
  pinMode(A2,INPUT); // set all used port to intput to save power
  pinMode(A3,INPUT); // set all used port to intput to save power
*/
  /*test leds*/
  int i=0;

  for(i=0;i<10;i++){
    digitalWrite(pinAbrir_1, HIGH);
    digitalWrite(pinCerrar_1, HIGH);  
    delay(1000);
    digitalWrite(pinAbrir_1, LOW);
    digitalWrite(pinCerrar_1, LOW);  
    delay(1000);
  }
  
  //system_sleep();
/*
  pinMode(A1,OUTPUT); // set all ports into state before sleep
  pinMode(A2,OUTPUT); // set all ports into state before sleep
  pinMode(A3,OUTPUT); // set all ports into state before sleep
  */
  for(i=0;i<4;i++){
    digitalWrite(pinHabilitarLecturas, HIGH);
    LecturaLuz();
    digitalWrite(pinHabilitarLecturas, LOW);
  }
  delay(3000);


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
  #if defined(__AVR_ATmega328P__)
    WDTCSR|= (1<<WDCE) | (1<<WDE);
    // set new watchdog timeout value
    WDTCSR = bb;
    WDTCSR|= _BV(WDIE);
  #else //attiny85
    WDTCR |= (1<<WDCE) | (1<<WDE);
    // set new watchdog timeout value
    WDTCR = bb;
    WDTCR |= _BV(WDIE);
  #endif
  
}

//****************************************************************  

// Watchdog Interrupt Service / is executed when  watchdog timed out

ISR(WDT_vect) {
  tiempo_dormido+=8000;
}

//**************************************************************** 
void LecturasSensores(){
  digitalWrite(pinHabilitarLecturas, HIGH);
  LecturaHumedad();
  LecturaLuz();
  digitalWrite(pinHabilitarLecturas, LOW);

}
//**************************************************************** 
void LecturaHumedad(){
  // read the sensor:
  sensorHumedad = analogRead(humedadPin);      
  sensorHumedad = (sensorHumedad/1023.00)*100.00;
}
//**************************************************************** 
void LecturaLuz(){
  // read the sensor:
  sensorLuz = analogRead(luzPin);
  sensorLuz = (sensorLuz/1023.00)*100.00;
  
  
  digitalWrite(pinAbrir_1, LOW);
  digitalWrite(pinCerrar_1, LOW);
  if( 70 < sensorLuz ){
    digitalWrite(pinAbrir_1, HIGH);
  }else if( sensorLuz <= 30 ){
    digitalWrite(pinCerrar_1, HIGH);
  }
  delay(3000);
  digitalWrite(pinAbrir_1, LOW);
  digitalWrite(pinCerrar_1, LOW);
  delay(3000);
}

//**************************************************************** 
//pulso de 30ms para abrir
void Abrir(){
    digitalWrite(pinAbrir_1, HIGH);
    delay(30);
    digitalWrite(pinAbrir_1, LOW);
}
//**************************************************************** 
//pulso de 30ms para cerrar
void Cerrar(){
    digitalWrite(pinCerrar_1, HIGH);
    delay(30);
    digitalWrite(pinCerrar_1, LOW);
}
//**************************************************************** 
//pulso de 30ms para abrir
void testAbrir(){
    int i;
    for(i=0;i<11;i++){
      digitalWrite(pinAbrir_1, HIGH);
      delay(300);
      digitalWrite(pinAbrir_1, LOW);
      delay(300);
    }
}
//**************************************************************** 
//pulso de 30ms para cerrar
void testCerrar(){
    int i;
    for(i=0;i<11;i++){
      digitalWrite(pinCerrar_1, HIGH);
      delay(300);
      digitalWrite(pinCerrar_1, LOW);
      delay(300);
    }
}


