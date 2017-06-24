#define INT_PIN_DER  2
#define INT_PIN_IZQ  4

#define PIN_PWM_IZQ_REV    9  
#define     PIN_PWM_IZQ   10  
#define PIN_PWM_DER_REV   11  
#define     PIN_PWM_DER    6  

#define        pin_encoder_derecho    4    //al blanco del encoder derecho
#define        pin_encoder_izquierdo  5    //al blanco del encoder izquierdo

float Dt[2][2]{{0.000,0.000},{0.000,0.000}};
int direccionMotor[2]={1,1};
long tiempo_ultimo_pulso[2]={0,0};



void setup() { 


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
} 

void loop(){
  analogWrite(PIN_PWM_IZQ,0);
  analogWrite(PIN_PWM_IZQ_REV,100);  //Serial.println(digitalRead(INT_PIN_IZQ)-0.5)*2);
  delay(1);
  Serial.print(Dt[0][0]);Serial.print(" v0: ");Serial.print(1/Dt[0][0], 4);Serial.print(" v1: ");Serial.println(1/Dt[0][1], 4);
  Serial.print(Dt[0][1]);Serial.print("a: ");Serial.println((1/Dt[0][1]-1/Dt[0][0])/Dt[0][1], 4);
  Serial.println("------");
  delay(2000);
  analogWrite(PIN_PWM_IZQ,0);
  analogWrite(PIN_PWM_IZQ_REV,0);  //Serial.println(digitalRead(INT_PIN_IZQ)-0.5)*2);
  delay(10000);
}
//**********************************************
//Lectura del encoder izquierdo
void IntEncoderIzq()
{ 
  direccionMotor[0]=digitalRead(pin_encoder_izquierdo);
  Dt[0][0] = Dt[0][1];
  Dt[0][1]=(micros()-tiempo_ultimo_pulso[0])/1000   ;                             // time between last state change and this state change
  tiempo_ultimo_pulso[0]=micros();  // update tiempo_ultimo_pulso[0] with time of most recent state change
}

//Lectura del encoder derecho
void IntEncoderDer()
{
  direccionMotor[1]=digitalRead(pin_encoder_derecho) ;
  Dt[1][0] = Dt[1][1];
  Dt[1][1]=(micros()-tiempo_ultimo_pulso[1])/1000000 ;                               // time between last state change and this state change
  tiempo_ultimo_pulso[1]=micros();  // update tiempo_ultimo_pulso[0] with time of most recent state change
}
