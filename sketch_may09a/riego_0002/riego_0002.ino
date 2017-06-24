//#include <TinkerKit.h>

/*
   The sensor value description
   0 ~ 300 dry soil
   300 ~ 700 humid soil
   700 ~ 950 in water
*/
//Pines
int regarPin = 2;

//sensores
int sensorHumedad = A5;
int sensorLuz =A4;

//constantes
int minHumedad=300;
int maxHumedad=500;
int limiteLuz=111;

unsigned long min_tiempo_regando = 300000; //un tiempo maximo de riego de 5 minutos
unsigned long max_tiempo_regando = 900000; //un tiempo maximo de riego de 15 minutos
unsigned long min_tiempo_entre_riegos = 28800000; //un tiempo minimo entre riegos de 8 horas

//variables
int humedad=0;
int luz=0;
boolean regando =false;
unsigned long  tiempo_Regando = 0 ;
unsigned long  tiempo_NO_Regando = min_tiempo_entre_riegos; //iniciamos con el maximo para que desde ya pueda regar
unsigned long  inicio_riego = 0 ;
unsigned long  paro_riego = 0 ;

//Set Up
void setup(){
  pinMode(regarPin, OUTPUT);
  //establecemos inicialmente 
  
}

//Loop
void loop(){
  regar_si_necesario();
  //millis % 60000 cada cierto tiempo actualizar los displays, con un contador, por ejemplo
}

//-----------------------------------
//Funciones
//-----------------------------------
void regar_si_necesario(){
  
  //lectura de la humedad y la luz
  humedad = analogRead (sensorHumedad);
  luz = analogRead(sensorLuz);

  if(regando){
    //Tiempo regando
    tiempo_Regando = millis()-inicio_riego;
  } else if(millis()>min_tiempo_entre_riegos){ // solo cuento el tiempo de paro de riego cuando lleva un tiempo funcionando 
    //Tiempo SIN regar
    tiempo_NO_Regando = millis()-paro_riego;
  }
  
  //activa riego
  if(humedad<minHumedad && luz<limiteLuz && tiempo_NO_Regando > min_tiempo_entre_riegos){
    
    if(!regando)
      inicio_riego = millis(); //Solo se actualiza la primera vez que entra en el bucle, al inicio del riego

      
    //regando
    regando=true;
    //válvula = abierta;
    digitalWrite(regarPin, HIGH);
    //Indicar con una señal que se ha iniciado riego y el tiempo que lleva
  }
  
  //desactiva riego
  if(humedad>maxHumedad || tiempo_Regando > max_tiempo_regando){ //cortamos si se supera la humedad o lleva maxTiempoRegando
    //NO regando
    regando=false;
    //válvula = cerrada;
    digitalWrite(regarPin, LOW);
    //ledAmarillo=low;
    paro_riego = millis();
    //Indicar con una señal que se ha parado el riego y el tiempo que lleva parado

  }
  

}

