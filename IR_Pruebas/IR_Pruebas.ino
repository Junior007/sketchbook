/* YourDuino.com Example Software Sketch
 IR Remote Kit Test
 Uses YourDuino.com IR Infrared Remote Control Kit 2
 http://arduino-direct.com/sunshop/index.php?l=product_detail&p=153
 based on code by Ken Shirriff - http://arcfn.com
 Get Library at: https://github.com/shirriff/Arduino-IRremote
 Unzip folder into Libraries. RENAME folder IRremote
 terry@yourduino.com */


#include "IRremote.h"

int receiver = 7; // pin 1 of IR receiver to Arduino digital pin 11

IRrecv irrecv(receiver);
decode_results results;

unsigned  long command;

void setup()   
{
  Serial.begin(9600);
  Serial.println("IR Receiver Raw Data + Button Decode Test");
  irrecv.enableIRIn(); // Start the receiver

}


void loop() 
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
  

}

/*-----( Declare User-written Functions )-----*/
void translateIR() // takes action based on IR code received

// describing Car MP3 IR codes 

{

  if(results.value!=0xFFFFFFFF){
      command=results.value;
  }
  
  switch(command)
  {

  case 0xFF18E7:  //2
    Serial.println("acelerar +"); 
    break;


  case 0xFF10EF:  //4
    Serial.println("izquierda"); 
    break;

  case 0xFF38C7:  //5
    Serial.println("Parar"); 
    break;

  case 0xFF5AA5:  //6
    Serial.println("derecha"); 
    break;

  case 0xFF4AB5:  //8
    Serial.println("acelerar -"); 
    break;
  default: 
    Serial.println(" other button   ");
    Serial.println(command,HEX);

  }
  delay(500);  
    /*
  case 0xFF42BD:  //7
    Serial.println(" 7              "); 
    break;

  case 0xFF7A85:  //3
    Serial.println(" 3              "); 
    break;  case 0xFF52AD:  //9
    Serial.println(" 9              "); 
    break;
  case 0xFFA25D:  
    Serial.println(" CH-            "); 
    break;

  case 0xFF629D:  
    Serial.println(" CH             "); 
    break;

  case 0xFFE21D:  
    Serial.println(" CH+            "); 
    break;

  case 0xFF22DD:  
    Serial.println(" PREV           "); 
    break;

  case 0xFF02FD:  
    Serial.println(" NEXT           "); 
    break;

  case 0xFFC23D:  
    Serial.println(" PLAY/PAUSE     "); 
    break;

  case 0xFFE01F:  
    Serial.println(" VOL-           "); 
    break;

  case 0xFFA857:  
    Serial.println(" VOL+           "); 
    break;

  case 0xFF906F:  
    Serial.println(" EQ             "); 
    break;

  case 0xFF6897:  
    Serial.println(" 0              "); 
    break;

  case 0xFF9867:  
    Serial.println(" 100+           "); 
    break;

  case 0xFFB04F:  
    Serial.println(" 200+           "); 
    break;

  case 0xFF30CF:  
    Serial.println(" 1              "); 
    break;
*/

}
