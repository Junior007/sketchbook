#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

int led = 12;

void setup(){
  //Bajar la frecuencia del reloj
  //CLKPR = (1<<CLKPCE);
  //CLKPR = B00001000;
  //b
  //CLKPR = 0x80;    // Tell the AtMega we want to change the system clock
  //CLKPR = 0x08;
  /*DDRD &= B00000011;       // set Arduino pins 2 to 7 as inputs, leaves 0 & 1 (RX & TX) as is
  DDRB = B00000000;        // set pins 8 to 13 as inputs
  PORTD |= B11111100;      // enable pullups on pins 2 to 7, leave pins 0 and 1 alone
  PORTB |= B11111111;      // enable pullups on pins 8 to 13
  pinMode(13,OUTPUT);      // set pin 13 as an output so we can use LED to monitor*/
  
//  Serial.begin(9600);
    //To reduce power, setup all pins as inputs with no pullups
  for(int x = 1 ; x < 18 ; x++){
    pinMode(x, INPUT);
    digitalWrite(x, LOW);
  }
  pinMode(led, OUTPUT);     


  //Power down various bits of hardware to lower power usage  
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();

  //Shut off ADC, TWI, SPI, Timer0, Timer1

  ADCSRA &= ~(1<<ADEN); //Disable ADC
  ACSR = (1<<ACD); //Disable the analog comparator
  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
  DIDR1 = (1<<AIN1D)|(1<<AIN0D); //Disable digital input buffer on AIN1/0
  
  power_twi_disable();
  power_spi_disable();
  power_usart0_disable();
  power_timer0_disable(); //Needed for delay_ms
  power_timer1_disable();
  //power_timer2_disable(); //Needed for asynchronous 32kHz operation

  //Setup TIMER2
  TCCR2A = 0x00;
  //TCCR2B = (1<<CS22)|(1<<CS20); //Set CLK/128 or overflow interrupt every 1s
  TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20); //Set CLK/1024 or overflow interrupt every 8s
  ASSR = (1<<AS2); //Enable asynchronous operation
  TIMSK2 = (1<<TOIE2); //Enable the timer 2 interrupt

  //Setup external INT0 interrupt
  EICRA = (1<<ISC01); //Interrupt on falling edge
  EIMSK = (1<<INT0); //Enable INT0 interrupt

  //System clock futzing
  //CLKPR = (1<<CLKPCE); //Enable clock writing
  //CLKPR = (1<<CLKPS3); //Divid the system clock by 256

  //Serial.begin(9600);  
  //Serial.println("32kHz Testing:");

  sei(); //Enable global interrupts

}
//Loop
void loop(){
  sleep_mode(); //Stop everything and go to sleep. Wake up if the Timer2 buffer overflows or if you hit the button
  
  //Serial.write("hola");
  delay(1000/256);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000/256);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW

}

