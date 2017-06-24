


/*CONSTANTES*/
//Pines
//Pines
int pinAbrir_1 = 12;
int pinCerrar_1 = 11;

int pinAbrir_2 = 10;
int pinCerrar_2 = 9;


//sensores
int sensorTemperatura = A3;
int sensorHumedad = A4;
int sensorLuz =A5;

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
float temperatura;

float temperatura0[10];

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
int estado_actual;

//Set Up
void setup(){
  
  //Preparamos pines
  pinMode(pinAbrir_1, OUTPUT);
  digitalWrite(pinAbrir_1, LOW);
  pinMode(pinCerrar_1, OUTPUT);
  digitalWrite(pinCerrar_1, LOW);
  //válvula = cerrada;
  Cerrar(1);
  /*  
  pinMode(pinAbrir_2, OUTPUT);
  digitalWrite(pinAbrir_2, LOW);
  pinMode(pinCerrar_2, OUTPUT);
  digitalWrite(pinCerrar_2, LOW);  
  //válvula = cerrada;
  Cerrar(2);
  */
  
  //Habilitamos comunicacion con el PC
  Serial.begin(9600);
  //Cargamos los arrays de las lecturas para las medias tomando NumeroLecturasMedia lecturas
  for(int i=0;i<NumeroLecturasMedia;i++){
      LecturaAmbiente();
  }
  
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
      Cerrar(1);
      //ledAmarillo=low;
      paro_riego = millis();
      //Indicar con una señal que se ha parado el riego y el tiempo que lleva parado
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
      Abrir(1);
      //Indicar con una señal que se ha iniciado riego y el tiempo que lleva
      Serial.println("Regando");
    }
  
  }
}

void Abrir(int valvula){
  if(valvula==1){
    digitalWrite(pinAbrir_1, HIGH);
    delay(30);
    digitalWrite(pinAbrir_1, LOW);
  }else if(valvula==2){
    digitalWrite(pinAbrir_2, HIGH);
    delay(30);
    digitalWrite(pinAbrir_2, LOW);
  }
}

void Cerrar(int valvula){
  if(valvula==1){
    digitalWrite(pinCerrar_1, HIGH);
    delay(30);
    digitalWrite(pinCerrar_1, LOW);
  }else   if(valvula==2){
    digitalWrite(pinCerrar_2, HIGH);
    delay(30);
    digitalWrite(pinCerrar_2, LOW);
  }
}



void LecturaAmbiente(){
  LecturaHumedad();
  LecturaLuz();
  LecturaTemperatura();
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


void   LecturaTemperatura(){
  int i;
  //Tomar la media de NumeroLecturasMedia valores
  for(i=NumeroLecturasMedia;i>0;i--){
    temperatura0[i] = temperatura0[i-1];
  }


  
  temperatura0[0]  = analogRead(sensorTemperatura) * 5.0/1024.0;

  temperatura0[0]=(temperatura0[0]-0.5)*10;
  
  temperatura=0;
  
  for(i=0;i<NumeroLecturasMedia;i++){
    temperatura += temperatura0[i];
  } 
  
  //Sacamos la media de temperaturas
  temperatura = temperatura/NumeroLecturasMedia;

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


