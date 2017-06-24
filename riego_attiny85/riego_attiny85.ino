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
//#include <avr/wdt.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>

#ifndef cbi

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

#endif

#ifndef sbi

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#endif

volatile boolean f_wdt=1;



/*CONSTANTES*/

//Pines
int pinAbrir_1 = 0;
int pinCerrar_1 = 1;

//sensores
int sensorHumedad = 4;
int sensorLuz =5;

//constantes
int min_humedad=10;
int max_humedad=80;
int limiteLuz=20; //20% de luz


//controlamos el tiempo mnimo y maximo de riego y el tiempo minimo entre riegos
unsigned long min_tiempo_regando = 300000; //un tiempo MINIMO de riego de 5 minutos
unsigned long max_tiempo_regando = 900000; //un tiempo maximo de riego de 15 minutos
unsigned long min_tiempo_entre_riegos = 18000000; //un tiempo minimo entre riegos de 5 horas

/*FIN DE CONSTANTES*/


//variables
float NumeroLecturasMedia = 10;

float luz;
float luz0[10];

float humedad;
float humedad0[10];

boolean regando =false;
boolean ocurrio_primer_riego = false; //si es el inicio del sistema no se ha de tener en cuenta el tiempo minimo entre riegos
unsigned long  tiempo_Regando = 0 ;
unsigned long  tiempo_NO_Regando = 0; 
unsigned long  inicio_riego = 0 ;
unsigned long  paro_riego = 0 ;
//****************************************************************

void setup(){
  pinMode(pinAbrir_1,OUTPUT);
  pinMode(pinCerrar_1,OUTPUT);

  Cerrar();
  setup_watchdog(9);

}
void test(){
  Cerrar();
  delay(2000);
  Abrir();
  delay(2000);
  Cerrar();
  delay(2000);
  Abrir();
 
}
//****************************************************************

//****************************************************************

//****************************************************************

void loop(){


  //****
  //test();
  //****
  
  //riego();
  
  f_wdt=0;       // reset flag

  pinMode(2,INPUT); // set all used port to intput to save power
  pinMode(3,INPUT); // set all used port to intput to save power
  pinMode(4,INPUT); // set all used port to intput to save power
  pinMode(5,INPUT); // set all used port to intput to save power

  system_sleep();

  pinMode(2,OUTPUT); // set all ports into state before sleep
  pinMode(3,OUTPUT); // set all ports into state before sleep
  pinMode(4,OUTPUT); // set all ports into state before sleep
  pinMode(5,OUTPUT); // set all ports into state before sleep


}

//-----------------------------------
//Funciones
//-----------------------------------
void riego(){


  //lectura de la humedad y la luz
  LecturaAmbiente();
  
  //REGANDO
  if(regando){
    
    //Tiempo regando
    tiempo_Regando = millis()-inicio_riego;

    //Para riego
    if(humedad>max_humedad && tiempo_Regando > min_tiempo_regando  || tiempo_Regando > max_tiempo_regando){ //cortamos si se supera la humedad siempre que no lleve mas del minimo de tiempo de riego o lleva max_tiempo_regando y 
      //NO regando
      regando=false;
      //válvula = cerrada;
      Cerrar();
      //ledAmarillo=low;
      paro_riego = millis();

    }
    
  }
  //NO REGANDO
  else{
   
    //Tiempo SIN regar
    tiempo_NO_Regando = millis()-paro_riego;//hay que tener en cuenta que de inicio no va a regar hasta pasado el tiempo entre riegos
  
    //activa riego
    if(humedad<min_humedad && luz<limiteLuz && (tiempo_NO_Regando > min_tiempo_entre_riegos || !ocurrio_primer_riego)){
      
      ocurrio_primer_riego=true; //nos permitia controlar que si no se habia regado por primera vez no tuviese en cuenta el tiempo entre riegos
      
      inicio_riego = millis(); //Solo se actualiza la primera vez que entra en el bucle, al inicio del riego
        
      //regando
      regando=true;
      //válvula = abierta;
      Abrir();
    }
  
  }
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
//*** LECTURA DE LOS SENSORES*************************************  
//****************************************************************  
void LecturaAmbiente(){
  LecturaHumedad();
  LecturaLuz();
}  
void LecturaHumedad(){  
  int i;
  //Tomar la media de NumeroLecturasMedia valores
  for(i=NumeroLecturasMedia;i>0;i--){
    humedad0[i] = humedad0[i-1];
  }

  humedad0[0] = analogRead (sensorHumedad)/1024.0*100.0 ;

  humedad=0;
  
  for(i=0;i<NumeroLecturasMedia;i++){
    humedad += humedad0[i];
  }

  humedad = humedad/NumeroLecturasMedia;

}  


void  LecturaLuz(){
  int i;
  //Tomar la media de NumeroLecturasMedia valores
  for(i=NumeroLecturasMedia;i>0;i--){
    luz0[i] = luz0[i-1];
  }
  
  luz0[0] = (analogRead(sensorLuz)/1023.00)*100.00;
  
  luz=0;
  
  for(i=0;i<NumeroLecturasMedia;i++){
    luz += luz0[i];
  } 
  
  //Sacamos la media de temperaturas
  luz = luz/NumeroLecturasMedia;

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





