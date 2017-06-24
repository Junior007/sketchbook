
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
#define        max_aceleracion        0.002

// define global variables here

float Dt[2][2]{{MAX_TIME,MAX_TIME},{MAX_TIME,MAX_TIME}}; //incrementos de tiempo el primer indice para izquierd (0) y derecha (0). El segundo para el anterior incremento en tiempo y calcular la aceleracion

int ms;

boolean lapso;

long tiempo_ultimo_pulso[2]={-1*MAX_TIME,-1*MAX_TIME};

float vel[2] = {0,0}; //vel[0] =izquierda; vel[1] =derecha
float vel_real[2]={0,0}; //vel_real[0] =izquierda; vel_real[1] =derecha

int pwm[2]={0,0};//pwm[0] =izquierda; pwm[1] =derecha

int direccionMotor[2]={1,1};

int contadorLapsos;

float VELOCIDAD = 100;
float     CURVA = 0;

int contadorLapsos_demo;
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
  VELOCIDAD = 30;//[0,115]
  CURVA     = 0;//[-1   -0.5   0   0.5   1]
  
  ms    = millis();
  lapso = false;
}


//*********************************************
void loop(){


  //En esta rutina colocar todo el cdigo
  loop2();  
  
  //Contamos lapsos de 100 ms aproximadamente
  if(millis() - ms>100){
    lapso=true;
    contadorLapsos++;
    ms=millis();
  }
  else{
    lapso=false;
 
  }
}

void loop2(){
  //Establece la velocidad de cada lado
  direccion();
  //Controla el motor
  motor();
}

void motor(){

  if(lapso){//cada 20 lapsos calibro la velocidad

    for(int i =0;i<2;i++){

      vel_real[i] = VelocidadEncoder(i);

      if(vel_real[i]<vel[i] && aceleracion(i)<max_aceleracion){
        pwm[i]++;
      }
      else if(vel[i] <vel_real[i] && aceleracion(i)>-1*max_aceleracion){
        pwm[i]--;
      }
      if(pwm[i] >255){
        pwm[i]=255;
      }
      else if(pwm[i] <-255){
        pwm[i]=-255;
      }
    }  
    //
    if(pwm[0]<0){
      analogWrite(PIN_PWM_IZQ,0);
      analogWrite(PIN_PWM_IZQ_REV,abs(pwm[0]));
    }
    else if(pwm[0]>0){
      analogWrite(PIN_PWM_IZQ_REV,0);
      analogWrite(PIN_PWM_IZQ,pwm[0]);
    }
    else{
      analogWrite(PIN_PWM_IZQ,0);
      analogWrite(PIN_PWM_IZQ_REV,0);
    }

    if(pwm[1]<0){
      analogWrite(PIN_PWM_DER,0);
      analogWrite(PIN_PWM_DER_REV,abs(pwm[1]));
    }
    else if(pwm[1]>0){
      analogWrite(PIN_PWM_DER_REV,0);
      analogWrite(PIN_PWM_DER,pwm[1]);
    }
    else{
      analogWrite(PIN_PWM_DER,0);
      analogWrite(PIN_PWM_DER_REV,0);
    }

    //Controlar carga bateria con PWM y Velocidad que coje
    if(_PWM(vel_real[1])/abs(pwm[1])<0.70){
      //Led derecho rojo encendido
      //analogWrite(LOW_BAT_DER,50);
    }
    if(_PWM(vel_real[0])/abs(pwm[0])<0.70){
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
    vel[0] = VELOCIDAD * (1.00 + CURVA * 2.00);//izquierda
    vel[1] = VELOCIDAD;
  }
  else if(CURVA>0){
    vel[0] = VELOCIDAD;//izquierda              
    vel[1] = VELOCIDAD * (1.00 - CURVA * 2.00);
  }
  else{
    vel[0] = VELOCIDAD;//izquierda
    vel[1] = VELOCIDAD;
  }
}
//**********************************************


float aceleracion(int i){
  Serial.println(Dt[i][1]);
  Serial.println(Dt[i][0]);
  Serial.println((1.0/Dt[i][1]-1.0/Dt[i][0])/Dt[i][1]);
  Serial.println("--------------------");

  return (1.0/Dt[i][1]-1.0/Dt[i][0])/Dt[i][1]; 
}

//**********************************************
float VelocidadEncoder(int i){

  int direccion=-1;
  if(direccionMotor[i]){  
    direccion = 1;
  }

  if((micros()-tiempo_ultimo_pulso[i])>MAX_TIME)
    return 0;
  else
    return direccion * (1.0/Dt[i][1]);
}

//**********************************************
//Lectura del encoder izquierdo
void IntEncoderIzq()
{ 
  direccionMotor[0]=digitalRead(pin_encoder_izquierdo);
  Dt[0][0] = Dt[0][1];
  Dt[0][1]=(micros()-tiempo_ultimo_pulso[0]) / 1000;                               // time between last state change and this state change
  tiempo_ultimo_pulso[0]=micros();  // update tiempo_ultimo_pulso[0] with time of most recent state change
}


//Lectura del encoder derecho
void IntEncoderDer()
{
  direccionMotor[1]=digitalRead(pin_encoder_derecho) ;
  Dt[1][0] = Dt[1][1];
  Dt[1][1]=(micros()-tiempo_ultimo_pulso[1]) / 1000;                               // time between last state change and this state change
  tiempo_ultimo_pulso[1]=micros();  // update tiempo_ultimo_pulso[0] with time of most recent state change
}

