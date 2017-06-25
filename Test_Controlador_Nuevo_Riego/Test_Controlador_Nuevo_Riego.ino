/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  analogReference(INTERNAL);//ref 1.1 volt
  //sensores
  pinMode(2, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  //digitalWrite(2,LOW); //abre lectura
  //digitalWrite(2,HIGH); //cierra lectura
  delay(1000);
  // read the input on analog pin 0:
  int tempSensorValor = analogRead(A4);
  int temp = tempSensorValor/9.31;
  // print out the value you read:
  Serial.println(temp);
  int luzSensorValor = analogRead(A5);
  // print out the value you read:
  Serial.println(luzSensorValor);
  Serial.println("Hola");
  delay(5000);        // delay in between reads for stability

  while (Serial.available() > 0) {
    if(Serial.read()=='1'){
      digitalWrite(2,HIGH);  
    }else if(Serial.read()=='0'){
      digitalWrite(2,LOW);
    }
    Serial.print(Serial.read());

  }

  
}
