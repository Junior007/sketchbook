


/*CONSTANTES*/
//Pines
int regarPin = 2;
int NOregarPin = 3;

//sensores
int sensorTemperatura = A3;
int sensorHumedad = A5;
int sensorLuz =A4;

//constantes
int min_humedad=10;
int max_humedad=80;
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
int numLecturas = 0;

float temperatura;
float temperatura0;
float temperatura1;
float temperatura2;

float luz;
float luz0;
float luz1;
float luz2;

int humedad;
int humedad0;
int humedad1;
int humedad2;

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
  pinMode(NOregarPin, OUTPUT);
  //establecemos valores iniciales 
  tiempo_display = millis();
  //Habilitamos comunicacion con el PC
  Serial.begin(9600);
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
      digitalWrite(regarPin, LOW);
      digitalWrite(NOregarPin, HIGH);
      //ledAmarillo=low;
      paro_riego = millis();
      //Indicar con una señal que se ha parado el riego y el tiempo que lleva parado
      EscrituraDatos();
      Serial.println("NO Regando");  
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
      digitalWrite(NOregarPin, LOW);
      //Indicar con una señal que se ha iniciado riego y el tiempo que lleva
      EscrituraDatos();
      Serial.println("Regando");
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


void LecturaAmbiente(){
  
  if(numLecturas<3)
    numLecturas++;
  
  LecturaHumedad();
  LecturaLuz();
  LecturaTemperatura();
}  



void LecturaHumedad(){  
  //TODO: Tomar la media de 3 valores

  humedad2 = humedad1;
  humedad1 = humedad0;
  humedad0 = analogRead (sensorHumedad)/680.0*100.0 ;
  //Sacamos la media de tres lecturas
  if(numLecturas == 1){
     humedad = humedad0;
  }else if(numLecturas == 2){
     humedad = (humedad0 + humedad1)/2.0;
    
  }else if(numLecturas == 3){
     humedad = (humedad0 + humedad1 + humedad2)/3.0;
    
  }  

}  


void   LecturaTemperatura(){
  

  
  //TODO: Tomar la media de 3 valores
  temperatura2 = temperatura1;
  temperatura1 = temperatura0;
  temperatura0  = analogRead(sensorTemperatura);
  temperatura0 = temperatura0 * 0.48828125;
  //Sacamos la media de tres temperaturas
  if(numLecturas == 1){
     temperatura = temperatura0;
  }else if(numLecturas == 2){
     temperatura = (temperatura0 + temperatura1)/2.0;
    
  }else if(numLecturas == 3){
     temperatura = (temperatura0 + temperatura1 + temperatura2)/3.0;
    
  }

}

void  LecturaLuz(){
  //TODO: Tomar la media de 3 valores
  luz2 = luz1;
  luz1 = luz0;
  luz0 = (analogRead(sensorLuz)/800.00)*100;//leer el valor del sensor de A0

  if(numLecturas == 1){
     luz = luz0;
  }else if(numLecturas == 2){
     luz = (luz0 + luz1)/2.0;
    
  }else if(numLecturas == 3){
     luz = (luz0 + luz1 + luz2)/3.0;
    
  }

} 

void EscrituraDatos(){  

  Serial.print("Humedad = ");
  Serial.print(humedad);
  Serial.println("%");

  Serial.print("T = ");
  Serial.print(temperatura);
  Serial.print("*C");
  Serial.println();


  Serial.print("L = ");
  Serial.print(luz);//I  
  Serial.println("%");


  Serial.print("tiempo_NO_Regando = ");
  Serial.print(tiempo_NO_Regando/1000);//I  
  Serial.println("s");


  Serial.print("tiempo_Regando = ");
  Serial.print(tiempo_Regando/1000);//I  
  Serial.println("s");


} 
