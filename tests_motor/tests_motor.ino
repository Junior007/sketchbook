#include <IRremote.h>
//https://learn.sparkfun.com/tutorials/ir-communication
int RECV_PIN = 52;
IRrecv irrecv(RECV_PIN);

decode_results results;

//Define IR codes
//#define IR_CODE_FORWARD 0xFF18E7
#define IR_CODE_FORWARD_3 0xF11C4F3F
#define IR_CODE_FORWARD_2 0xD16A9212
//#define IR_CODE_FORWARD_1 0xF11C4F3F

#define IR_CODE_BACKWARDS 0xFF4AB5
//#define IR_CODE_TURN_LEFT 0xFF10EF
#define IR_CODE_TURN_LEFT 0xA716803F //x2 veces
//#define IR_CODE_TURN_RIGHT 0xFF5AA5
#define IR_CODE_TURN_RIGHT 0x13417DCD //x2 veces
#define IR_CODE_STOP 0xFF38C7
//#define IR_CODE_CONTINUE 0XFFFFFFFF
#define IR_CODE_CONTINUE 0X3C285047



boolean isActing = false; //If the robot is executing command from remote
long timer;
const long TIME_OUT = 150;
unsigned long prev_res = IR_CODE_CONTINUE;

// define IO pins
#define INT_PIN_DER    3  // D2 pin for external interrupt 0 //amarillo encoder derecho
#define INT_PIN_IZQ    2  // D3 pin for external interrupt 1  //amarillo encoder izquierdo
#define pin_encoder_derecho    5    //al blanco del encoder derecho
#define pin_encoder_izquierdo  4    //al blanco del encoder izquierdo

#define PIN_PWM_IZQ_REV     9 //marrón, indica direccion
#define     PIN_PWM_IZQ    10 
#define PIN_PWM_DER_REV    11 //marrón, indica direccion
#define     PIN_PWM_DER    12 

#define        MAX_TIME 10000.0
#define        max_aceleracion        0.05

// define global variables here
float Dt_izq   = MAX_TIME;         
float Dt_der   = MAX_TIME;
float Dt_izq_0 = MAX_TIME;
float Dt_der_0 = MAX_TIME;


unsigned int ms=0;

long tiempo_ultimo_pulso_izq=-1*MAX_TIME;
long tiempo_ultimo_pulso_der=-1*MAX_TIME;

float v_izq;
float v_der;

int pwm_izq;
int pwm_der;

int dir_izq;
int dir_der;

unsigned int contador;

float VELOCIDAD = 0;
float     CURVA = 0;

//*********************************************  
void setup()
{
  pinMode(PIN_PWM_IZQ,OUTPUT);
  pinMode(PIN_PWM_IZQ_REV,OUTPUT);
  pinMode(PIN_PWM_DER,OUTPUT);
  pinMode(PIN_PWM_DER_REV,OUTPUT);

  pinMode(pin_encoder_izquierdo, INPUT); 
  pinMode(pin_encoder_derecho, INPUT); 

  attachInterrupt(0, IntEncoderIzq, RISING);//RISING  
  attachInterrupt(1, IntEncoderDer, RISING);//RISING

  
  Serial.begin(250000);
  Serial.println("Inicio");


  
}


//*********************************************
void loop(){

  if(abs(millis()-ms)>5){
    ms=millis();
    contador++;
  }

 //If the robot does not receive any command, stop it
/* if (isActing && (millis() - timer >= TIME_OUT)) {
   //Robot.motorsStop();
   isActing = false;
 }
 */

  
  if(contador%200==0){
     
    Serial.print(VIzq());
    Serial.print("-");
    Serial.println(VDer());

    pwm_der=100;


    analogWrite(PIN_PWM_DER,abs(pwm_der));
    analogWrite(PIN_PWM_DER_REV,pwm_der<0);
    /*
    analogWrite(PIN_PWM_IZQ,abs(pwm_izq));
    analogWrite(PIN_PWM_IZQ_REV,pwm_izq<0);
   Serial.print(pwm_der);//, 4);
   Serial.print("-");//, 4);
   Serial.println(pwm_izq);//, 4) ;*/
  }
 
}

//**********************************************

float a_izq(){

  return 750000.0 * (1.0/Dt_izq-1.0/Dt_izq_0)/Dt_izq; 


}
float a_der(){

  return 750000.0 * (1.0/Dt_der-1.0/Dt_der_0)/Dt_der; 


}
//**********************************************
float VIzq(){

  int direccion=-1;
  if(dir_izq){  
    direccion = 1;
  }

  if((micros()-tiempo_ultimo_pulso_izq)>MAX_TIME)
    return 0;
  else
    return direccion * (750000.0/Dt_izq);
}
float VDer(){

  int direccion=-1;
  if(dir_der){  
    direccion = 1;
  }

  if((micros()-tiempo_ultimo_pulso_der)>MAX_TIME)
    return 0;
  else
    return direccion * (750000.0/Dt_der);
}

//**********************************************
//Lectura del encoder izquierdo
void IntEncoderIzq()
{ 
  dir_izq=digitalRead(pin_encoder_izquierdo);
  Dt_izq_0 = Dt_izq;
  Dt_izq=micros()-tiempo_ultimo_pulso_izq;                               // time between last state change and this state change
  tiempo_ultimo_pulso_izq=micros();  // update tiempo_ultimo_pulso_izq with time of most recent state change
}


//Lectura del encoder derecho
void IntEncoderDer()
{
  dir_der=digitalRead(pin_encoder_derecho) ;
  Dt_der_0 = Dt_der;
  Dt_der=micros()-tiempo_ultimo_pulso_der;                               // time between last state change and this state change
  tiempo_ultimo_pulso_der=micros();  // update tiempo_ultimo_pulso_izq with time of most recent state change
}



