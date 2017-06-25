#define TRIGGER    9
#define ECHO    8

unsigned int tiempo, distancia;

void setup()
{
    Serial.begin(9600);
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);
}

void loop()
{
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);
    // Calcula la distancia midiendo el tiempo del estado alto del pin ECHO
    tiempo = pulseIn(ECHO, HIGH);
    // La velocidad del sonido es de 340m/s o 29 microsegundos por centimetro
    distancia= tiempo/58;
    //manda la distancia al monitor serie
    Serial.print(distancia);
    Serial.println(" cm"); 
    
    
    delay(1000);
}
