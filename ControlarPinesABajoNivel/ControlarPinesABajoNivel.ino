
/*
Testeando con arduino nano 328
D (digital pins 0 to 7)
B (digital pin 8 to 13)
C (analog input pins)
*/

//int led = 4;
int i=2;

void setup(){

  Serial.begin(9600);
  Serial.println("Hola");
  DDRD |=16; //Pin 4 a output

  //DDRD = B11111110;  // sets Arduino pins 1 to 7 as outputs, pin 0 as input
  //DDRD = DDRD | B11111100;



}

void loop(void){

    PORTD |=16; //Pin 4 a HIGH
    delay(1000);
    PORTD &= ~16; //PIN 4 a LOW
    delay(1000);

  /*
  if(i>=13)
    return;
  for(i=2;i<=13;i++){   
    pinMode(i, OUTPUT); 
    Serial.print("PIN = ");
    Serial.println(i);
    Serial.print("DDRB = ");
    Serial.println(DDRB,BIN);
    Serial.print("DDRD = ");
    Serial.println(DDRD,BIN);
    delay(1000);  
  }*/
}
