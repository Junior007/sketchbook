

// define IO pins

#define INT_PIN_IZQ    2  // D2 pin for external interrupt 0
#define INT_PIN_DER    3  // D3 pin for external interrupt 1

#define PIN_PWM_IZQ_REV    6 //  D7  left  motor direction
#define     PIN_PWM_IZQ    9 //  D9  left  motor PWM
#define PIN_PWM_DER_REV   10 //  D8  right motor direction
#define     PIN_PWM_DER   11 //  D10 right motor PWM
#define        MAX_TIME 10.00

// define global variables here

volatile float Dt_izq=100000;         
volatile float Dt_der=100000;         

volatile unsigned long tiempo_ultimo_pulso_izq=MAX_TIME;
volatile unsigned long tiempo_ultimo_pulso_der=MAX_TIME; 

//
float VELOCIDAD = 0;
float CURVA     = 0; //[-1   -0.5   0   0.5   1]

int izq_pwm; 
int der_pwm;
float izq_vel; 
float der_vel;
float izq_pwm_pos=1.00;
float der_pwm_pos=1.00;
float izq_pwm_neg=1.00;
float der_pwm_neg=1.00;

float FI;
float FD;

//*********************************************  
void setup()
{

  pinMode(PIN_PWM_IZQ_REV,OUTPUT);
  pinMode(PIN_PWM_DER_REV,OUTPUT);
  pinMode(PIN_PWM_IZQ,OUTPUT);
  pinMode(PIN_PWM_DER,OUTPUT);

  attachInterrupt(0,IntEncoderIzq,CHANGE); 
  attachInterrupt(1,IntEncoderDer,CHANGE); 

  Serial.begin(9600); 
}

//*********************************************
void loop()
{ 
  Serial.println("Adelante");
  mover(20,0.5);
  delay(2000);
  mover(0,0);
  delay(10000);
  mover(20,-0.5);
  delay(2000);
  mover(0,0);
  delay(10000);
}

void mover(int velocidad, int curva){
  VELOCIDAD = velocidad; //[0,200]  o por encima
  CURVA     = curva; //[-1   -0.5   0   0.5   1]
  motor();
 
}

//************************************************************
void motor(){
  for(int i=0;i<5;i++){
    calc_PWM_motor();
    transferir_PWM_motor();
    compensar();
  }
  calc_PWM_motor();
  Serial.print("VIzq: ");Serial.print(VIzq());    Serial.print("VDer: ");Serial.println(VIzq());  
  Serial.print("izq_pwm_neg: ");Serial.print(izq_pwm_neg);    Serial.print("der_pwm_neg: ");Serial.println(der_pwm_neg);  
  Serial.print("izq_pwm_pos: ");Serial.print(izq_pwm_pos);    Serial.print("der_pwm_pos: ");Serial.println(der_pwm_pos);  


}
//**************************************************************
void calc_PWM_motor(){

  int VELOCIDAD_X_FI;
  int VELOCIDAD_X_FD;

  /*
  CURVA = [-1   -0.5   0   0.5   1]
  */
  if(CURVA <0){
    FI = (1 + CURVA * 2);
    FD = 1;
  }else if(CURVA>0){
    FI = 1;
    FD = (1 - CURVA * 2);
  }else{
    FI=1;
    FD=1;
  }
  
  VELOCIDAD_X_FI = VELOCIDAD * FI;
  VELOCIDAD_X_FD = VELOCIDAD * FD;
  
  //El motor no se porta igual yendo hacia adelante y hacia atrás, por eso hacemos el cálculo de dos factores
  if(VELOCIDAD_X_FI>0)
    izq_pwm = VELOCIDAD_X_FI * izq_pwm_pos;              
  else if(VELOCIDAD_X_FI<0)
    izq_pwm = VELOCIDAD_X_FI * izq_pwm_neg;
  else
    izq_pwm = 0;
    
  if(VELOCIDAD_X_FD>0)
    der_pwm = VELOCIDAD_X_FD * der_pwm_pos;
  else if(VELOCIDAD_X_FD<0)
    der_pwm = VELOCIDAD_X_FD * der_pwm_neg;
  else
    der_pwm = 0;

}


//******************************************************************
void transferir_PWM_motor(){

  //PWM para los motores
  if(izq_pwm<0){
    if(izq_pwm<-255) izq_pwm = -255;
    analogWrite(PIN_PWM_IZQ,0);
    analogWrite(PIN_PWM_IZQ_REV,abs(izq_pwm));
  }else if(izq_pwm>0){
    if(izq_pwm>255) izq_pwm = 255;
    analogWrite(PIN_PWM_IZQ,izq_pwm);
    analogWrite(PIN_PWM_IZQ_REV,0);
  }else{
    analogWrite(PIN_PWM_IZQ,0);
    analogWrite(PIN_PWM_IZQ_REV,0);
  }
  
  if(der_pwm<0){
    if(der_pwm<-255) der_pwm = -255;
    analogWrite(PIN_PWM_DER,0);
    analogWrite(PIN_PWM_DER_REV,abs(der_pwm));
  }else if(der_pwm>0){
    if(der_pwm>255) der_pwm = 255;
    analogWrite(PIN_PWM_DER,der_pwm);
    analogWrite(PIN_PWM_DER_REV,0);
  }else{
    analogWrite(PIN_PWM_DER,0);
    analogWrite(PIN_PWM_DER_REV,0);
  }
  
}  
//corregimos velocidad de los motores
void compensar(){
  float Vi = VIzq();
  float Vd = VDer();

  if(izq_pwm !=0 &&  Vi!=0){
    if(izq_pwm>0)
      izq_pwm_pos = abs(izq_pwm / Vi);
    else if(izq_pwm<0)
      izq_pwm_neg = abs(izq_pwm / Vi);
  }
  
  if(der_pwm !=0 &&  Vd!=0){
    if(der_pwm>0)
      der_pwm_pos = abs(der_pwm / Vd);
    else if(der_pwm<0)
      der_pwm_neg = abs(der_pwm / Vd);
  }
  

}
//**********************************************
float VIzq(){
  if((micros()-tiempo_ultimo_pulso_izq)>=MAX_TIME)
    return 0;
  else
    return 3000/Dt_izq;
}
float VDer(){
  if((micros()-tiempo_ultimo_pulso_der)>=MAX_TIME)
    return 0;
  else
    return 3000/Dt_der;
}
//**********************************************
int OperarSobreModulo(int valor, int operador){
  return (abs(valor) + operador) * abs(valor)/valor;
}
//**********************************************
//Lectura del encoder izquierdo
void IntEncoderIzq()
{ 
  Dt_izq=micros()-tiempo_ultimo_pulso_izq;                               // time between last state change and this state change
  tiempo_ultimo_pulso_izq=micros();                                    // update tiempo_ultimo_pulso_izq with time of most recent state change
  
}


//Lectura del encoder derecho
void IntEncoderDer()
{
  Dt_der=micros()-tiempo_ultimo_pulso_der;                               // time between last state change and this state change
  tiempo_ultimo_pulso_der=micros();                                      // update tiempo_ultimo_pulso_izq with time of most recent state change
}
