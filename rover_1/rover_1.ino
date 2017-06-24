
// define IO pins
#define INT_PIN_DER    2  // D2 pin for external interrupt 0
#define INT_PIN_IZQ    3  // D3 pin for external interrupt 1

#define PIN_PWM_IZQ_REV     9 
#define     PIN_PWM_IZQ    10 
#define PIN_PWM_DER_REV    11 
#define     PIN_PWM_DER     6 

#define        MAX_TIME 10000.0
#define        pin_encoder_derecho    4    //al blanco del encoder derecho
#define        pin_encoder_izquierdo  5    //al blanco del encoder izquierdo
#define        max_aceleracion        0.05

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


int contador_demo;
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

  Serial.begin(115200);
  Serial.println("Inicio");


  //la velocidad que he conseguido con la pila cargada 100% es hasta algo menos de 120
  VELOCIDAD = -20;//[0,115]
  CURVA = 0;//[-1   -0.5   0   0.5   1]
  
  lapso=true;
}


//*********************************************
void loop(){

  if(ms>5){
    ms=0;
    lapso=true;
    contador++;
  }
  else{
    ms=millis();
    lapso=false;
  }

  direccion();
  motor();
  /*
  if(contador%80==0){
   Serial.print(a_der(), 4);
   Serial.print("   :   ");
   Serial.println(a_izq(), 4) ;
  }*/
  /*
  if(contador%1000==0){
  /*
    if(contador_demo==0){
     VELOCIDAD = 50;//[0,115]
      CURVA = 0;//[-1   -0.5   0   0.5   1]
    }else if(contador_demo==1){
      CURVA = 1;//[-1   -0.5   0   0.5   1]
    }else if(contador_demo==2){
      VELOCIDAD = -50;//[0,115]
      CURVA =0;//[-1   -0.5   0   0.5   1]
    }else if(contador_demo==3){
      CURVA =1;//[-1   -0.5   0   0.5   1]
    }else{
      contador_demo=-1;
    }
    
    contador_demo++;


  }*/

}


void motor(){

  float v_der_real = VDer();
  float v_izq_real = VIzq();

  if(contador%20==0){

    if(v_der_real<v_der && a_der()<max_aceleracion){
      pwm_der++;
    }
    else if(v_der <v_der_real && a_der()>-1*max_aceleracion){
      pwm_der--;
    }
    if(v_izq_real <v_izq && a_izq()<max_aceleracion){
      pwm_izq++;

    }
    else if(v_izq <v_izq_real && a_izq()>-1*max_aceleracion){
      pwm_izq--;
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
    //
    if(pwm_izq<0){
      analogWrite(PIN_PWM_IZQ,0);
      analogWrite(PIN_PWM_IZQ_REV,abs(pwm_izq));
    }
    else if(pwm_izq>0){
      analogWrite(PIN_PWM_IZQ_REV,0);
      analogWrite(PIN_PWM_IZQ,pwm_izq);
    }
    else{
      analogWrite(PIN_PWM_IZQ,0);
      analogWrite(PIN_PWM_IZQ_REV,0);
    }
  
    if(pwm_der<0){
      analogWrite(PIN_PWM_DER,0);
      analogWrite(PIN_PWM_DER_REV,abs(pwm_der));
    }
    else if(pwm_der>0){
      analogWrite(PIN_PWM_DER_REV,0);
      analogWrite(PIN_PWM_DER,pwm_der);
    }
    else{
      analogWrite(PIN_PWM_DER,0);
      analogWrite(PIN_PWM_DER_REV,0);
    }
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



