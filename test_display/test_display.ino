//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

/*
     -a-
    f   b
    - g  -
    e   c 
     -d- 

*/
//gfedcbap
int digits[] = {
  B01111110, //0
  B00001100, //1
  B10110110, //2
  B10011110, //3
  B11001100, //4
  B11011010, //5
  B11111010, //6
  B00001110, //7
  B11111110, //8
  B11011110, //9



  B11101110, //10 ->A
  B11101100, //11 ->H
  B01110000, //12 ->L
  B01111110, //13 ->O




}; 


void setup() {
  
  Serial.begin(9600); // The serial monitor is used for debugging.
  
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {

  writeDigit(11);
  delay(1000);
  writeDigit(13);
  delay(1000);
  writeDigit(12);
  delay(1000);
  writeDigit(10);
  delay(1000);
  
  
  for(int digit=0;digit<10;digit++){
    writeDigit(digit);
    delay(1000);
  }

  // count from 0 to 255 and display the number 
  // on the LEDs
  /*
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
*/
}

void writeDigit(int digit) {
  Serial.print("-----");  //Do some pretty formatting in the serial monitor

  //This next part is taken from Carlyn Maw & Tom Igoe's 74hc595 example at: http://arduino.cc/en/Tutorial/ShiftOut ,
  //If your getting started with using SPI, shift registers or anything that this project requires, do
  //the examples on this page. They are invaluable.
  //------------------BEGIN THE MAGIC!!!--------------------
  digitalWrite(latchPin, LOW); // First the latch pin must be pulled low this tells the register to recieve
  //shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  
  shiftOut(dataPin,clockPin,MSBFIRST,digits[digit]); //Using the value for the data and clock pins, 
                                                         //telling the arduino wich Bitorder, and the payload
                                                         //the entire byte is shifted into the 74hc595.

  Serial.print(digit); //Tell me over the serial monitor which number just sent over
  Serial.print(" | "); // pretty pretty formatting
  Serial.println(digits[digit] ,BIN); // Show me the actual bits that were sent out
  digitalWrite(latchPin,HIGH); // Pull the latch pin high. This will cause the register to do two things:
                               // 1) Ignore the clock & data lines. 2) Push the stored byte to the output pins
  
  
}
