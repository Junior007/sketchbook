float temperatura;
float luz;
float humedad;

int sensorTemperatura = A3;
int sensorHumedad = A4;
int sensorLuz =A5;


void setup()
{
  Serial.begin(9600);
  analogReference(INTERNAL);
}

void loop()
{
  LecturaLuz();
  LecturaTemperatura();
  LecturaHumedad();
  delay(3000);
  delay(3000); 
}  
void LecturaHumedad(){  
  humedad = analogRead (sensorHumedad)/1024.0*100.0;
  Serial.print("Humedad = ");
  Serial.print(humedad);
  Serial.println("%");

}  


void   LecturaTemperatura(){
   temperatura = analogRead(sensorTemperatura) * 5.0/1024.0;
   temperatura = (temperatura-0.5)*10;
  
 
  //temperatura = temperatura/ 9.31; 
  //temperatura = temperatura; 
  
  Serial.print("T = ");
  Serial.print(temperatura);
  Serial.print("*C");
  Serial.println();
}

void  LecturaLuz(){
  luz = (analogRead(sensorLuz)/1024.00)*100;//leer el valor del sensor de A0
  Serial.print("L = ");
  Serial.print(luz);//I  
  Serial.println("%");
}  

