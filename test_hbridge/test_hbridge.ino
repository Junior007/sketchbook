
const int pinCerrar_1 = 7;
const int pinAbrir_1 = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Modo test");
}

void loop() {

  digitalWrite(pinAbrir_1, LOW);
  digitalWrite(pinCerrar_1, LOW);
  // put your main code here, to run repeatedly:
  Serial.println("Abierto");
  pinMode(pinAbrir_1,OUTPUT);
  digitalWrite(pinAbrir_1, HIGH);
  delay(3000);
  digitalWrite(pinAbrir_1, LOW);
  Serial.println("Cerrado");
  delay(10);
  pinMode(pinCerrar_1,OUTPUT);
  digitalWrite(pinCerrar_1, HIGH);
  delay(3000);
  digitalWrite(pinCerrar_1, LOW);
delay(10);
}
