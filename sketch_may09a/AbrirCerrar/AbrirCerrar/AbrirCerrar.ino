


/*CONSTANTES*/
//Pines
int pinAbrir_1 = 12;
int pinCerrar_1 = 11;

int pinAbrir_2 = 10;
int pinCerrar_2 = 9;


//Set Up
void setup(){


  //Preparamos pines
  pinMode(pinAbrir_1, OUTPUT);
  digitalWrite(pinAbrir_1, LOW);
  pinMode(pinCerrar_1, OUTPUT);
  digitalWrite(pinCerrar_1, LOW);
  pinMode(pinAbrir_2, OUTPUT);
  digitalWrite(pinAbrir_2, LOW);
  pinMode(pinCerrar_2, OUTPUT);
  digitalWrite(pinCerrar_2, LOW);


  //Cerramos valvulas
  //La funcin cerrar ha de verificar que no haya flujo de agua
  Cerrar(1);
  Cerrar(2);
  
  //Habilitamos comunicacion con el PC
  Serial.begin(9600);
}

//Loop
void loop(){
  delay(5000);
  Serial.println("Abrir");
  Abrir(1);
  Abrir(2);
  delay(5000);
  Serial.println("Cerrar");
  Cerrar(1);
  Cerrar(2);
}


void Abrir(int valvula){
  if(valvula==1){
    digitalWrite(pinAbrir_1, HIGH);
    delay(30);
    digitalWrite(pinAbrir_1, LOW);
  }else if(valvula==2){
    digitalWrite(pinAbrir_2, HIGH);
    delay(30);
    digitalWrite(pinAbrir_2, LOW);
  }
}

void Cerrar(int valvula){
  if(valvula==1){
    digitalWrite(pinCerrar_1, HIGH);
    delay(30);
    digitalWrite(pinCerrar_1, LOW);
  }else   if(valvula==2){
    digitalWrite(pinCerrar_2, HIGH);
    delay(30);
    digitalWrite(pinCerrar_2, LOW);
  }
}
