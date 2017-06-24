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
int sensorTemperatura =A3;

//constantes
int minHumedad=300;
int maxHumedad=500;
int maxLuz=111;

//variables
int humedad=0;
int luz=0;
float temperatura = 0.0;

boolean regando =false;

//Set Up
void setup(){
  Serial.begin(9600);
  pinMode(regarPin, OUTPUT);
}
//Loop
void loop(){

  //lectura de la humedad
  humedad = analogRead (sensorHumedad);
  luz = analogRead(sensorLuz);
  temperatura = analogRead(sensorTemperatura);
  temperatura = (5.0 * temperatura * 100.0)/1024.0;

  Serial.print(temperatura);
  Serial.println('C');
  delay(3000);
  Serial.print(humedad);
  Serial.println('%');
  delay(3000);
  Serial.print(luz);
  Serial.println('-');
  delay(3000);

  if(humedad<minHumedad && luz<maxLuz){
    regar();

  }
  //Paramos durante 5s
  delay(5000); 
}
//-----------------------------------
//Funciones
//-----------------------------------
void regar(){
    while(humedad<maxHumedad ){
      //regando
      regando=true;
      //válvula = abierta;
      digitalWrite(regarPin, HIGH);
      //ledAmarillo=high;
      //paramos 1 minuto
      //delay(60000);

      //lectura de la humedad
      humedad = analogRead(sensorHumedad);
    }
    regando = false;
    //válvula = cerrada;
    digitalWrite(regarPin, LOW);
    //ledAmarillo=low;
}
