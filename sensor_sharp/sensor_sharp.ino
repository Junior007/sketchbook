
int val;
int dist;
int redpin=A0;
void setup() {
    pinMode(redpin,OUTPUT);
    Serial.begin(9600);
}
void loop() {
    val=analogRead(redpin);
    dist = 4800/(val - 20);
    Serial.println(dist);
    delay(1000);
}
