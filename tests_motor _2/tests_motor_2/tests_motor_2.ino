
// define IO pins

#define INT_PIN_IZQ    2  // D2 pin for external interrupt 0
#define INT_PIN_DER    3  // D3 pin for external interrupt 1

#define PIN_PWM_IZQ_REV    6 //  D7  left  motor direction
#define     PIN_PWM_IZQ    9 //  D9  left  motor PWM
#define PIN_PWM_DER_REV   10 //  D8  right motor direction
#define     PIN_PWM_DER   11 //  D10 right motor PWM
#define        MAX_TIME 500000

// define global variables here

volatile float tiempo_pulsos_izq=100000;         
volatile float tiempo_pulsos_der=100000;         

volatile unsigned long tiempo_ultimo_pulso_izq=MAX_TIME;
volatile unsigned long tiempo_ultimo_pulso_der=MAX_TIME; 

//
int   _PWM      = 0;
float VELOCIDAD = 0;
float CURVA     = 0; //[-1   -0.5   0   0.5   1]

int izq_pwm; 
int der_pwm;
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
  
/*
  Serial.println("Delante e Izquierdo");
  _PWM = 30;
  CURVA = 0.5; //[-1   -0.5   0   0.5   1]
  motor();
  delay(2000);
  _PWM = 0;
  CURVA = -0.5; //[-1   -0.5   0   0.5   1]
  motor();
  delay(2000);
  Serial.println("Delante y derecho");
  _PWM = 30;
  CURVA = -0.5; //[-1   -0.5   0   0.5   1]
  motor();
  delay(2000);*/
    Serial.println("Adelante");
  _PWM = 255;
  VELOCIDAD=200; //[0,200]  o por encima
  CURVA = 0; //[-1   -0.5   0   0.5   1]
  motor();
  delay(3000);
  _PWM = 0;
  CURVA = 0; //[-1   -0.5   0   0.5   1]
  motor();
  delay(10000);
 /*
tiempo_pulsos_izq  tiempo_pulsos_der
  /*
  _PWM = 50;
  CURVA = -1; //[-1   -0.5   0   0.5   1]
  motor();
  delay(6000);  
  _PWM = 50;
  CURVA = 0; //[-1   -0.5   0   0.5   1]
  motor();
  delay(6000);  
  _PWM = 50;
  CURVA = 1; //[-1   -0.5   0   0.5   1]
  motor();
  delay(6000);  */
  
}

//************************************************************
void motor(){
  
  calc_PWM_motor();
  compensar_y_transferir_PWM_motor();
}
//**************************************************************
void calc_PWM_motor(){

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
  
  izq_pwm = _PWM * FI;              
  der_pwm = _PWM * FD;
  
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
void compensar_y_transferir_PWM_motor(){
	
	/*
				% desviaci�n		
	255	2,55		1	2		2,55
	255	5,1		    2	5		2,55
	255	7,65		3	7		2,55
	255	10,2		4	10		2,55
	255	12,75		5	12		2,55
	255	15,3		6	15		2,55
	255	17,85		7	17		2,55
	255	20,4		8	20		2,55
	255	22,95		9	23		2,55
	255	25,5		10	25		2,55

        izq_pwm/der_pwm = FI/FD
	*/

  transferir_PWM_motor();

  //TO DO:mejorar esto
  if(FI == 0 || FD == 0 ) return;
  if(VIzq() == 0 || VDer() == 0 ) return;

  float factor = FI / FD;
  int iteraciones = abs((abs(VIzq() / VDer())  - abs(factor))/ factor) * 255;
  Serial.print("factor")     ;  Serial.println(factor);
  Serial.print("iteraciones");  Serial.println(iteraciones);
  
  for(int i = 0;i<iteraciones && abs(abs(VIzq() / VDer())  - abs(factor))>0.01;i++){
    
    
    Serial.print(VIzq());Serial.print("        ");  Serial.println(VDer());
    
    if(abs(VIzq() / VDer())  > abs(factor)){
       Serial.print("1-")     ;  Serial.println(abs(abs(VIzq() / VDer())  - abs(factor)));

      //Compensar a derechas
      der_pwm = OperarSobreModulo(der_pwm, 1);  Serial.println("//le sumamos 1 pero al mdulo");
    }else if(abs(VIzq() / VDer())  < abs(factor)){
       Serial.print("2-")     ;  Serial.println(abs(abs(VIzq() / VDer())  - abs(factor)));
      //Compensar a izquierdas
      izq_pwm = OperarSobreModulo(izq_pwm, 1); Serial.println("//le sumamos 1 pero al mdulo");
    }
		
    transferir_PWM_motor();

  }
  Serial.println("_______________________________________");
}
//**********************************************
float VIzq(){
  if((micros()-tiempo_ultimo_pulso_izq)>=MAX_TIME)
    return 0;
  else
    return 1000000/tiempo_pulsos_izq;
}

float VDer(){
  if((micros()-tiempo_ultimo_pulso_der)>=MAX_TIME)
    return 0;
  else
    return 1000000/tiempo_pulsos_der;
}
//**********************************************
int OperarSobreModulo(int valor, int operador){
  return (abs(valor) + operador) * abs(valor)/valor;
}
//Lectura del encoder izquierdo
void IntEncoderIzq()
{ 
  tiempo_pulsos_izq=micros()-tiempo_ultimo_pulso_izq;                               // time between last state change and this state change
  tiempo_ultimo_pulso_izq=micros();                                    // update tiempo_ultimo_pulso_izq with time of most recent state change
  
}


//Lectura del encoder derecho
void IntEncoderDer()
{
  tiempo_pulsos_der=micros()-tiempo_ultimo_pulso_der;                               // time between last state change and this state change
  tiempo_ultimo_pulso_der=micros();                                      // update tiempo_ultimo_pulso_izq with time of most recent state change
}
