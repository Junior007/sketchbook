unsigned int ms=0;


unsigned int contador;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Inicio");
}

void loop() {


  if(ms>5){
    ms=0;
    contador++;
  }
  else{
    ms=millis();
  }
  //If the robot does not receive any command, stop it
  
 
  if(contador%50==0){

   Serial.println(contador);//, 4);

  }

}
