
// define IO pins
#define INT_PIN_IZQ    2  // D2 pin for external interrupt 0
#define INT_PIN_DER    3  // D3 pin for external interrupt 1

#define PIN_PWM_IZQ_REV    6 //  D7  left  motor direction
#define     PIN_PWM_IZQ    9 //  D9  left  motor PWM
#define PIN_PWM_DER_REV   10 //  D8  right motor direction
#define     PIN_PWM_DER   11 //  D10 right motor PWM

#define        MAX_TIME 10000.0000
#define        pin_encoder_derecho   4   //al amarillo del encoder derecho
#define        pin_encoder_izquierdo 5    //al amarillo del encoder izquierdo
// define global variables here
float Dt_izq   = MAX_TIME;         
float Dt_der   = MAX_TIME;
float Dt_izq_0 = MAX_TIME;
float Dt_der_0 = MAX_TIME;


int ms=0;
boolean lapso;

long tiempo_ultimo_pulso_izq=-1*MAX_TIME;
long tiempo_ultimo_pulso_der=-1*MAX_TIME;

float v_izq;
float v_der;

int pwm_izq;
int pwm_der;

int dir_izq;
int dir_der;

int contador;

float VELOCIDAD = 0;
float     CURVA = 0;

//*********************************************  
void setup()
{
  pinMode(PIN_PWM_IZQ,OUTPUT);
  pinMode(PIN_PWM_IZQ_REV,OUTPUT);
  pinMode(PIN_PWM_DER,OUTPUT);
  pinMode(PIN_PWM_DER_REV,OUTPUT);
  attachInterrupt(0, IntEncoderIzq, CHANGE);//RISING
  attachInterrupt(1, IntEncoderDer, CHANGE);//RISING

  Serial.begin(9600);
  Serial.println("Inicio");
  

  //la velocidad que he conseguido con la pila tal y como est es hasta algo menos de 120
  VELOCIDAD = 10;//[0,115]
  CURVA = 0;

  lapso=true;
}


//*********************************************
void loop(){
  direccion();
  motor();
}


void motor(){
  

  
  if(ms>5){
    ms=0;
    lapso=true;
    contador++;
  }else{
    ms=millis();
    lapso=false;
  }

  /*
  if((contador %10000)==0){
    v_izq+=4;
    v_der+=4;
  }
  */

  float v_der_real = VDer();
  float v_izq_real = VIzq();

  float max_aceleracion = 0.0100;
  
  
  /*if(contador%30==0){

    if(v_der_real > v_izq_real){
      if(v_der_real > v_der){
        pwm_der--;
      }else{
        pwm_izq++;
      }
    }else if(cv_der_real < v_izq_real){
      if(v_der_real < v_der){
        pwm_der++;
      }else{
        pwm_izq--;
      }
    }
    
  }else*/
  if(contador%10==0){
    
      if(v_der_real<v_der && a_der()<max_aceleracion){
        pwm_der++;
      }else if(v_der <v_der_real && a_der()>-1*max_aceleracion){
        pwm_der--;
      }
      if(v_izq_real <v_izq && a_izq()<max_aceleracion){
        pwm_izq++;
      }else if(v_izq <v_izq_real && a_izq()>-1*max_aceleracion){
        pwm_izq--;
      }
     
  }
  //if pwm_izq>0
  if(pwm_izq<0){
    analogWrite(PIN_PWM_IZQ,0);
    analogWrite(PIN_PWM_IZQ_REV,abs(pwm_izq));
  }else if(pwm_izq>0){
    analogWrite(PIN_PWM_IZQ_REV,0);
    analogWrite(PIN_PWM_IZQ,pwm_izq);
  }else{
    analogWrite(PIN_PWM_IZQ,0);
    analogWrite(PIN_PWM_IZQ_REV,0);
  }
  
  if(pwm_der<0){
    analogWrite(PIN_PWM_DER,0);
    analogWrite(PIN_PWM_DER_REV,abs(pwm_der));
  }else if(pwm_der>0){
    analogWrite(PIN_PWM_DER_REV,0);
    analogWrite(PIN_PWM_DER,pwm_der);
  }else{
    analogWrite(PIN_PWM_DER,0);
    analogWrite(PIN_PWM_DER_REV,0);
  }
  /*
  for(int i=0;i<150;i+=20){
    pwm_izq=i;
    pwm_der=i;


    Serial.print(a_izq());
    Serial.print(" : ");
    Serial.print(a_der());
    Serial.println();
    Serial.print(VIzq());
    Serial.print(" : ");
    Serial.print(VDer());
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    delay(10000);
  }*/
/*
  if(contador % 100 == 0){
    Serial.print(pwm_der);
    Serial.print(":");
    Serial.println(pwm_izq);
  
  
  
    Serial.print(VIzq());
    Serial.print(":");
    Serial.println(VDer());
  }*/
}

/*
int _PWM(float v){
  
  int pwm;
  
  //ajuste por mnimos cuadradados con medidas de PWNM/velocidad
  pwm=69-3*v+0.04*pow(v,2);
 
  
  if(pwm>255)
    pwm=255;  
    
  return pwm;  
}
*/

void direccion(){
  /*
  CURVA = -0.5; //[-1   -0.5   0   0.5   1]
  */
  if(CURVA <0){
    v_izq = VELOCIDAD * (1.00 + CURVA * 2.00);
    v_der = VELOCIDAD;
  }else if(CURVA>0){
    v_izq = VELOCIDAD;              
    v_der = VELOCIDAD * (1 - CURVA * 2);
  }else{
    v_izq = VELOCIDAD;
    v_der = VELOCIDAD;
  }
}
//**********************************************

float a_izq(){
  
  return 7500000.0000 * (1.0000/Dt_izq-1.0000/Dt_izq_0)/Dt_izq;  


}
float a_der(){
  
  return 7500000.0000 * (1.0000/Dt_der-1.0000/Dt_der_0)/Dt_der;  


}
//**********************************************
float VIzq(){
  if((micros()-tiempo_ultimo_pulso_izq)>=MAX_TIME)
    return 0;
  else
    return 750000.0000/Dt_izq;
}
float VDer(){
  if((micros()-tiempo_ultimo_pulso_der)>=MAX_TIME)
    return 0;
  else
    return 750000.0000/Dt_der;
}

//**********************************************
//Lectura del encoder izquierdo
void IntEncoderIzq()
{ 
  dir_izq=-1;
  if(digitalRead(pin_encoder_izquierdo)) dir_izq=1;

  Dt_izq_0 = Dt_izq;
  Dt_izq=micros()-tiempo_ultimo_pulso_izq;                               // time between last state change and this state change
  tiempo_ultimo_pulso_izq=micros();                                    // update tiempo_ultimo_pulso_izq with time of most recent state change
}


//Lectura del encoder derecho
void IntEncoderDer()
{
  dir_der=-1;
  if(digitalRead(pin_encoder_derecho)) dir_der=1;
  Dt_der_0 = Dt_der;
  Dt_der=micros()-tiempo_ultimo_pulso_der;                               // time between last state change and this state change
  tiempo_ultimo_pulso_der=micros();                                      // update tiempo_ultimo_pulso_izq with time of most recent state change
}


