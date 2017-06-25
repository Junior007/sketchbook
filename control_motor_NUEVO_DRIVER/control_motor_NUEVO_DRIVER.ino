#include <IRremote.h>
//https://learn.sparkfun.com/tutorials/ir-communication
int RECV_PIN = 52;
IRrecv irrecv(RECV_PIN);

decode_results results;



//mando gyr, canal C
#define IR_CODE_TURN_LEFT 0x8C4BA750
#define IR_CODE_TURN_LEFT_1 0x23E38F69

#define IR_CODE_TURN_RIGHT   0xA90A0674  //Usa dos códigos, tiene dos modos en el canal c
#define IR_CODE_TURN_RIGHT_1 0xB7B891DB

#define IR_CODE_FORWARD_C   0x932768D1
#define IR_CODE_FORWARD_C_1 0xFB8F80B8

#define IR_CODE_BACKWARD_C   0x739780D9
#define IR_CODE_BACKWARD_C_1 0x8ED1BF61


boolean isActing = false; //If the robot is executing command from remote
long timer;
const long TIME_OUT = 500;


// define IO pins
#define INT_PIN_DER    3  // D2 pin for external interrupt 0 //amarillo encoder derecho
#define INT_PIN_IZQ    2  // D3 pin for external interrupt 1  //amarillo encoder izquierdo
#define pin_encoder_derecho    5    //al blanco del encoder derecho
#define pin_encoder_izquierdo  4    //al blanco del encoder izquierdo


#define PIN_PWM_DER_REV  9 //11 //marrón, indica direccion
#define PIN_PWM_IZQ_REV  10 //12  //marrón, indica direccion
#define     PIN_PWM_DER  11 //9 
#define     PIN_PWM_IZQ  12 //10  

#define        MAX_TIME 10000.0//100000.0
//#define        max_aceleracion        0.05

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

unsigned int incremento1cada5ms;

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
  
  
  pinMode(RECV_PIN,INPUT); // 

  irrecv.enableIRIn(); // Start the receiver
  
  //la velocidad que he conseguido con la pila cargada 100% es hasta algo menos de 120
  VELOCIDAD = 0;//[0,115]
  CURVA = 0;//[-1   -0.5   0   0.5   1]
  
}


//*********************************************
void loop(){
  
  recibeComando();
  
  if(abs(millis()-ms)>5){
    ms=millis();
    incremento1cada5ms++;
  }

 //If the robot does not receive any command, stop it
 if (isActing && (millis() - timer >= TIME_OUT)) {
    changeAction(0,0);
    isActing = false;
 }
 

    
  if(incremento1cada5ms%10==0){
    direccion();
    motor(); 
  }

  //
  if(incremento1cada5ms%10==0){
  /*
    Serial.print(VIzq());
    Serial.print("-");
    Serial.println(VDer());
   
   
   Serial.print(pwm_der);//, 4);
   Serial.print(" || ");//, 4);
   Serial.println(pwm_izq);//, 4) ;*/ 
  }
 
}
//
void recibeComando(){
  if (irrecv.decode(&results)) {
    unsigned long result =results.value;
    irrecv.resume(); // Receive the next value
    processResult(result);
  }
}
//**************************************
//mandamos la instrucción al rover
void processResult(unsigned long res) {

  float VEL = 80.00;
  
  if(IR_CODE_TURN_LEFT ==res || IR_CODE_TURN_LEFT_1==res)
    changeAction(VEL, -1); //Turn the robot left
  else if(IR_CODE_TURN_RIGHT ==res || IR_CODE_TURN_RIGHT_1==res)
    changeAction(VEL, 1); //Turn the robot left
  else if(IR_CODE_FORWARD_C ==res || IR_CODE_FORWARD_C_1==res)
    changeAction(VEL, 0); //Turn the robot left
  else if(IR_CODE_BACKWARD_C ==res || IR_CODE_BACKWARD_C_1==res)
    changeAction(-VEL, 0); //Turn the robot left
}
void changeAction(float VEL, float CUR) {
  VELOCIDAD=VEL;
  CURVA=CUR;

  timer = millis();
  isActing = true;
}
//**********************************************
float max_aceleracion(float error){
  //Serial.println(pow(error,2)/1000);
  //permitimos más o menos aceleración
  return pow(error,2)/1000;

}
void motor(){

  float v_der_real = VDer();
  float v_izq_real = VIzq();

  if(v_der_real<v_der && a_der()<max_aceleracion(v_der_real - v_der)){
    pwm_der++;
  }
  else if(v_der <v_der_real && a_der()>-1*max_aceleracion(v_der_real - v_der)){
    pwm_der--;
  }
  //
  ;
  if(v_izq_real <v_izq && a_izq()<max_aceleracion(v_izq_real -v_izq)){
    pwm_izq++;

  }
  else if(v_izq <v_izq_real && a_izq()>-1*max_aceleracion(v_izq_real -v_izq)){
    pwm_izq--;
  }
  //Si la velocidad ha de ser cero y ya no se mueve, cortamos la alimentación
  if(v_der_real==v_der){
    pwm_der=0;
  }
  //
  if(v_izq_real ==v_izq){
    pwm_izq=0;

  }  
  //
  if(pwm_izq >255){
    pwm_izq=255;
  }
  else if(pwm_izq <-255){
    pwm_izq=-255;
  }
  //
  if(pwm_der >255){
    pwm_der=255;
  }
  else if(pwm_der <-255){
    pwm_der=-255;
  }
  
  analogWrite(PIN_PWM_IZQ,abs(pwm_izq));
  digitalWrite(PIN_PWM_IZQ_REV,pwm_izq<0);


  analogWrite(PIN_PWM_DER,abs(pwm_der));
  digitalWrite(PIN_PWM_DER_REV,pwm_der<0);

  //Controlar carga bateria con PWM y Velocidad que coje
  if(_PWM(v_der_real)/abs(pwm_der)<0.70){
    //Led derecho rojo encendido
    //analogWrite(LOW_BAT_DER,50);
  }
  if(_PWM(v_izq_real)/abs(pwm_izq)<0.70){
    //Led izquierdo rojo encendido
    //analogWrite(LOW_BAT_IZQ,50);

  }
}

//***************
int _PWM(float v){
 
 int pwm;
 
 //ajuste por mnimos cuadradados con medidas de PWNM/velocidad
 pwm=69.0-3.0*v+0.04*pow(v,2);
 
 
 if(pwm>255)
 pwm=255;  
 
 return pwm;  
}
//***************
void direccion(){
  /*
  CURVA = -0.5; //[-1   -0.5   0   0.5   1]
   */
  if(CURVA <0){
    v_izq = VELOCIDAD * (1.00 + CURVA * 2.00);
    v_der = VELOCIDAD;
  }
  else if(CURVA>0){
    v_izq = VELOCIDAD;              
    v_der = VELOCIDAD * (1.00 - CURVA * 2.00);
  }
  else{
    v_izq = VELOCIDAD;
    v_der = VELOCIDAD;
  }
}
//**********************************************

float a_izq(){
  if((micros()-tiempo_ultimo_pulso_der)>MAX_TIME)
    return 0;
  else
    return 750000.0 * (1.0/Dt_izq-1.0/Dt_izq_0)/Dt_izq; 
}
float a_der(){
  if((micros()-tiempo_ultimo_pulso_der)>MAX_TIME)
    return 0;
  else
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





