float temp;
float luminosidad;

int tempPin = 3;
int luzPin =5;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Luminosidad();
  delay(3000);
  Temperatura();
  delay(3000);
}  


void   Temperatura(){
  temp = analogRead(tempPin);
  temp = temp * 0.48828125;
  Serial.print("T = ");
  Serial.print(temp);
  Serial.print("*C");
  Serial.println();
}

void  Luminosidad(){
  luminosidad = (analogRead(luzPin)/800.00)*100;//leer el valor del sensor de A0
  Serial.print("L = ");
  Serial.print(luminosidad);//I  
  Serial.println("%");
}  

