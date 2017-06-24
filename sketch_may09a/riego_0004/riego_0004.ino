//#include <TinkerKit.h>

/*
   The sensor value description
   0 ~ 300 dry soil
   300 ~ 700 humid soil
   700 ~ 950 in water
*/

/*CONSTANTES*/
//Pines
int regarPin = 2;

//sensores
int sensorHumedad = A5;
int sensorLuz =A4;

//constantes
int min_humedad=300;
int max_humedad=500;
int limiteLuz=20; //20% de luz


//controlamos el tiempo mnimo y maximo de riego y el tiempo minimo entre riegos
unsigned long min_tiempo_regando = 300000; //un tiempo MINIMO de riego de 5 minutos
unsigned long max_tiempo_regando = 900000; //un tiempo maximo de riego de 15 minutos
unsigned long min_tiempo_entre_riegos = 18000000; //un tiempo minimo entre riegos de 5 horas
//Tiempo para refrescar display y cambiar su estdo
unsigned long tiempo_permanencia_display= 30000; //30 segundos para que el dispaly muestre otra lectura

//Estados del display
enum estados_display
{
  mostrar_temperatura,
  mostrar_humedad,
  mostrar_tiempo_ultimo_riego,
  cambiar_al_primero
};

/*FIN DE CONSTANTES*/



//variables
int humedad=0;
int luz=0;
boolean regando =false;
boolean ocurrio_primer_riego = false; //si es el inicio del sistema no se ha de tener en cuenta el tiempo minimo entre riegos
unsigned long tiempo_display; //tiempo desde el ultimo cambio en el display
unsigned long  tiempo_Regando = 0 ;
unsigned long  tiempo_NO_Regando = 0; 
unsigned long  inicio_riego = 0 ;
unsigned long  paro_riego = 0 ;
int estado_actual;

//Set Up
void setup(){
  pinMode(regarPin, OUTPUT);
  //establecemos valores iniciales 
  tiempo_display = millis();
}

//Loop
void loop(){
  riego();
  //millis % 60000 cada cierto tiempo actualizar los displays, con un contador, por ejemplo
}

//-----------------------------------
//Funciones
//-----------------------------------
void riego(){


  //lectura de la humedad y la luz
  humedad = analogRead (sensorHumedad);
  luz = (analogRead(sensorLuz)/800.00)*100; //estimacion para rango entro 0 y 100%. 100 sera de dia
  
  //REGANDO
  if(regando){
    
    //Tiempo regando
    tiempo_Regando = millis()-inicio_riego;

    //Para riego
    if(humedad>max_humedad && tiempo_Regando > min_tiempo_regando  || tiempo_Regando > max_tiempo_regando){ //cortamos si se supera la humedad siempre que no lleve mas del minimo de tiempo de riego o lleva max_tiempo_regando y 
      //NO regando
      regando=false;
      //válvula = cerrada;
      digitalWrite(regarPin, LOW);
      //ledAmarillo=low;
      paro_riego = millis();
      //Indicar con una señal que se ha parado el riego y el tiempo que lleva parado
  
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
      digitalWrite(regarPin, HIGH);
      //Indicar con una señal que se ha iniciado riego y el tiempo que lleva
    }
  
  }
}

//cambia el estado del display
void display(){
  
  if (tiempo_permanencia_display < millis() - tiempo_display) {
    tiempo_display = millis();
    
    estado_actual++;
    
    if (estado_actual >= cambiar_al_primero){
      //estado_actual = estados_display.mostrar_temperatura;
      
      
      switch (estado_actual)
      {
        case mostrar_temperatura:
          break;
        case mostrar_humedad:
          break;
        case mostrar_tiempo_ultimo_riego:
          break;
      }
    }
  
  }
}
