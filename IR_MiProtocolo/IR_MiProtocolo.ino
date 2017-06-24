#define IRpin_PIN PIND
#define IRpin 7
unsigned int receivingCode = 0;      //variable donde cargamos lo que recibimos
unsigned int receivedCode = 0;     //commando completo recibido
unsigned long startTime = 0;
unsigned long lastTimeData = 0;  //tiempo  pasado desde el último valor alto
int MAX_LONG_DATA = 32; //La variable que recibe el código debe de ser unsing y no de mayor tamaño
long MAX_LOW_TIME = 55000;
long BIT_WIDE = 550;//us
int actualData = -1;




void setup(void) {
  Serial.begin(9600);
  Serial.println("Inicio");
}

void loop(void) {
  GetCommand();
}



static void GetCommand()
{

    unsigned long lag;
    unsigned long microseconds;
    int newData;
    int bitData = 0;

    microseconds = micros();

    if (actualData == -1)
    {

        actualData = 0;
        receivingCode = 0;
        startTime = microseconds;
        lastTimeData = microseconds;
    }

    lag = (microseconds - startTime);
    newData = (int)(lag / BIT_WIDE);
    

    if (IRpin_PIN & _BV(IRpin))
    {   // _BV(IRpin) = (1 << IRpin))
        // pin is still HIGH and there are a new high-pulse
        bitData= 0;
    }
    if (! (IRpin_PIN & _BV(IRpin)))
    {   // _BV(IRpin) = (1 << IRpin))
        bitData= 1;
        lastTimeData = microseconds; //Guardamos referencia del último nivel a alto
    }

    if (actualData<newData  && newData < MAX_LONG_DATA  )
    {
        receivingCode = (receivingCode << 1) | bitData;
        actualData = newData;
    }

    if (newData >= MAX_LONG_DATA){
      
      if(receivingCode>0){
        Serial.println(receivingCode);

      }
      

      actualData = -1;
      
      
    }




/*    if (newData >= MAX_LONG_DATA || (microseconds - lastTimeData) > MAX_LOW_TIME)
    {  

        if (receivingCode > 0)
        {/*
            //imprimir código receivedCode (¿y una espera solo para testear?)
            Serial.println("receivedCode: ");
            Serial.println(receivingCode);
            Serial.println("newData: ");
            Serial.println(newData);

*//*
        }
        //new command
        receivedCode = receivingCode;
        receivingCode=0;
        //newCommand= true
        actualData = -1;


    }*/
}
