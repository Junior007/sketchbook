
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


//---------------------------------------------- Set PWM frequency for D5 & D6 -------------------------------
 
//TCCR0B = TCCR0B & B11111000 | B00000001;    // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz
//TCCR0B = TCCR0B & B11111000 | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
//TCCR0B = TCCR0B & B11111000 | B00000011;    // set timer 0 divisor to    64 for PWM frequency of   976.56 Hz (The DEFAULT)
TCCR0B = TCCR0B & B11111000 | B00000100;    // set timer 0 divisor to   256 for PWM frequency of   244.14 Hz
//TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz
 
 
//---------------------------------------------- Set PWM frequency for D9 & D10 ------------------------------
 
//TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
//TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
//TCCR1B = TCCR1B & B11111000 | B00000011;    // set timer 1 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
TCCR1B = TCCR1B & B11111000 | B00000100;    // set timer 1 divisor to   256 for PWM frequency of   122.55 Hz
//TCCR1B = TCCR1B & B11111000 | B00000101;    // set timer 1 divisor to  1024 for PWM frequency of    30.64 Hz
 
//---------------------------------------------- Set PWM frequency for D3 & D11 ------------------------------
 
//TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
//TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
//TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
//TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
//TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
//TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz


}


//*********************************************
void loop(){

      int i = 1;
  
      vel[i] = 0.7;
      
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
    
    Serial.print(pwm[1]);
    aceleracion(i);
    VelocidadEncoder(i);

}



//**********************************************


float aceleracion(int i){
  Serial.print("a: ");
  Serial.println((1.0/Dt[i][1]-1.0/Dt[i][0])/Dt[i][1],10);
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
  else{
    Serial.print("v: ");
    Serial.println(direccion * (1.0/Dt[i][1]),10);
    Serial.println("--------------------");

    return direccion * (1.0/Dt[i][1]);
  }
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

