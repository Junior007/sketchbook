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

//Test configuracion

boolean test = false;
//Pines sensores
const int temp_p = A4;
const int luz_p = A0;
//pin habilita lectura
const int pinHabilitarSensores = 2;
const boolean lecturaHabilitada = LOW;
//pines h-bridge
const int pinCerrar_1 = 7;
const int pinAbrir_1 = 8;
//****************
//estado del riego
int estado = 'A';
int cDormido=0;
int nDormidoMediaHora=225;// 225 sueños son 8*225 = 1800s = media hora
//****************
float tempAnterior = 0; // Guarda la temperatura anterior
float umbralNoche = 2.0; // Define un umbral de caída de temperatura
int bajadasSuaves = 0;  //Contamos varias bajadas de temperatura
//************
//float tempActual = 0;
float sumaTemp = 0;
float tempMedia = 0;
float cuentaLecturas = 0;
//************************
int ciclosRiego = 0;
int divisorMediaHora = 0;
//****************************
int diasSinRiego = 0;

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
  if(test){//for testing purpose
   Serial.println("wake up"); 
  }    
}
//*************************************************************

void setup() {

if(test){//for testing purpose
  nDormidoMediaHora=2;//2 minutos modificamos el nmeroo de sueños si testeamos para que vaya ms rpido
  Serial.begin(9600);
  Serial.println("Modo test");
}  

  analogReference(INTERNAL);//ref 1.1 volt, necesary for LM35 temp sensor


  digitalWrite(pinAbrir_1, LOW);
  digitalWrite(pinCerrar_1, LOW);
  Abrir();
  delay(2000);
  Cerrar();

  setup_watchdog(9);
}

// the loop routine runs over and over again forever:
void loop() {

  DeshabilitarSensores(); //cierra lectura

  system_sleep();

  cDormido++;


  if(test){
    Serial.println("En el loop :");
    Serial.println((char)estado);
    Serial.println("cDormido");
    Serial.println(cDormido);
    delay(1000);
  }

  switch(estado) {
      case 'A': //Estado de alerta, está verificando la temperatura para ver cuando cae
          estadoA();
          break;
      case 'B': //Pendiente de que pase una hora para regar
          estadoB();
          break;
      case 'C': //regando
          estadoC();
          break;
      case 'D': //duerme durante 22 hora
          estadoD();
          break;
      case 'E': //estado extra para regar si han pasado 7 dias sin regar por temperaturas bajas
          estadoE();
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
    HabilitarSensores();
    
    //Dark?
    if(noche()){
      estado ='B';
    }
    //Desactiva la lectura sensores
    //disable sensor feed
    DeshabilitarSensores();

    
    if(test){
      Serial.println("En A :");
      Serial.println("tempAnterior");
      Serial.println(tempAnterior);
      Serial.println("tempMedia");
      Serial.println(tempMedia);
      
      delay(1000);
    }
  }
}
//**************************************************************** 
void estadoB(){
  //Se duerme y despierta hasta pasado una hora
  //sleep and wake up until one hour
  if(cDormido>=nDormidoMediaHora * 2){

    cDormido = 0;

    estado ='C';//pasa al estado de riego

  }
}
//**************************************************************** 
void estadoC(){
  
  if(cDormido==1){
    //Establecemos tiempo de riego y reiniciamos variables temperatura
    sumaTemp = 0;
    cuentaLecturas = 0;
    
    divisorMediaHora = map(tempMedia, 15, 40, 350, 6);
    //40 grados => riego durante 30 minutos / 6 = 5 minutos

    ciclosRiego = nDormidoMediaHora/divisorMediaHora;  //divisorMediaHora =6 =>5 minutos regando

    if(ciclosRiego == 0){

      diasSinRiego++;

      cDormido = 0;

      if(diasSinRiego>4){
        estado ='E';//pasa al estado de riego semanal
      }else{
        estado ='D';//pasa al estado de sueño profundo sin regar
      }

    }
  }
  if(ciclosRiego > 0){
    if(cDormido == 1){
      diasSinRiego = 0;
      Abrir();  
    }
    if(cDormido>=(ciclosRiego)){
      cDormido=0;
      Cerrar();
      estado ='D';//pasa al estado de sueño profundo
    }
  }

}
//**************************************************************** 
void estadoD(){
  if(cDormido>=(nDormidoMediaHora * 42)){//21 horas durmiendo y despertndose
    cDormido=0;
    estado ='A'; //pasa al estado de alerta
  }
}
//**************************************************************** 
void estadoE(){
  if(cDormido==1){
    diasSinRiego = 0;
    Abrir();  
  }
  if(cDormido>=(nDormidoMediaHora/6)){//divisorMediaHora =6 =>5 minutos regando
    cDormido=0;
    Cerrar();
    estado ='D';//pasa al estado de sueño profundo
  }
}
//**************************************************************** 
//pulso de 30ms para abrir
void Abrir(){
  pinMode(pinAbrir_1,OUTPUT);
  digitalWrite(pinAbrir_1, HIGH);
  delay(30);
  digitalWrite(pinAbrir_1, LOW);
}
//**************************************************************** 
//pulso de 30ms para cerrar
void Cerrar(){
  pinMode(pinCerrar_1,OUTPUT);
  digitalWrite(pinCerrar_1, HIGH);
  delay(30);
  digitalWrite(pinCerrar_1, LOW);
}


//*******************************************
float temp(){
  //valor = 0    => 0V   => 2ºC
  //valor = 1023 => 1.1V => 2+(1.1/0.01) ºC = 112 º C. Por encima ya no podemos leer con esta configuracion
  //valor = ???? => 1.48V =>150ºC
  // T  = 2 + (1.1 x valor)/(1023 * 0.01) 
  if(test){
    
    Serial.print("Esperando temp: ");
    delay(3000);
    if (Serial.available() > 0) {
      float dato = Serial.parseFloat();  // Lee un número decimal (float)
      Serial.print("Número recibido: ");
      Serial.println(dato, 4);  // Imprime con 4 decimales de precisión
      return dato;
    }
  }

  float valor = analogRead(temp_p);
  float temp = 2.0 + (1.1 * valor)/(1023.0 * 0.01) ;
  return temp;
}

//**************************************************************
boolean noche() {
  float tempActual = temp();

  // Evaluamos la bajada suave
  float gradiente = tempAnterior - tempActual;

  if (gradiente >= 0.5)
      bajadasSuaves++;
  else if (gradiente < -0.5)
      bajadasSuaves = 0;

  boolean esNoche = (tempAnterior - tempActual) >= umbralNoche; 

  esNoche= esNoche || bajadasSuaves >= 3;
  
  tempAnterior = tempActual; // Guarda el valor actual para la próxima comparación

  sumaTemp += tempActual;
  cuentaLecturas++;
  tempMedia = sumaTemp/cuentaLecturas;

  return esNoche;
}

//**************************************************************** 
void HabilitarSensores(){
  pinMode(pinHabilitarSensores,OUTPUT);    
  pinMode(A0, INPUT);    

  digitalWrite(pinHabilitarSensores,lecturaHabilitada);
  delay(2);  //para estabilizar la lectura
}
//**************************************************************** 
void DeshabilitarSensores(){
  pinMode(pinHabilitarSensores,OUTPUT); 
  digitalWrite(pinHabilitarSensores,!lecturaHabilitada);
}
//**********************
