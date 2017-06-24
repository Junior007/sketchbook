int led = 12;

void setup(){
  //a
  //CLKPR = (1<<CLKPCE);
  //CLKPR = B00001000;
  //b
  CLKPR = 0x80;    // Tell the AtMega we want to change the system clock
  CLKPR = 0x08;
//  Serial.begin(9600);
  pinMode(led, OUTPUT);     

}
//Loop
void loop(){
  //Serial.write("hola");
  delay(1000/256);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000/256);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
}
