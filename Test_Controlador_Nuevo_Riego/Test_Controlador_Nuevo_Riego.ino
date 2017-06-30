const int temp_p = A4;
const int luz_p = A5;
const int lectura_p = 2;


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  analogReference(INTERNAL);//ref 1.1 volt
  //sensores
  pinMode(lectura_p, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  //digitalWrite(2,LOW); //abre lectura
  //digitalWrite(2,HIGH); //cierra lectura
  delay(1000);


  Serial.println(temp());

  Serial.println(luz());
  
  Serial.println("OK");
  
  delay(5000);        // delay in between reads for stability

  while (Serial.available() > 0) {
    if(Serial.read()=='1'){
      digitalWrite(lectura_p,HIGH);  
    }else if(Serial.read()=='0'){
      digitalWrite(lectura_p,LOW);
    }
    Serial.print(Serial.read());

  }

  
}

//
float temp(){
  //valor = 0    => 0V   => 2ºC
  //valor = 1023 => 1.1V => 2+(1.1/0.01) ºC = 112 º C. Por encima ya no podemos leer con esta configuracion
  //valor = ???? => 1.48V =>150ºC
  // T  = 2 + (1.1 x valor)/(1023 * 0.01) 
  float valor = analogRead(temp_p);
  float temp = 2.0 + (1.1 * valor)/(1023.0 * 0.01) ;
  return temp;
}
//
int luz(){
  int valor = analogRead(luz_p);
  return valor;
}




