byte buf[1024]  ;


void setup(){
  Serial.begin(9600);
  for(int i=0;i<1024;i++){
    buf[i] = i % 255;
  }
}

void loop(){

  int bytesSent = Serial.write(buf, sizeof(buf));
   
  delay(5000);
  
  for(int i=0;i<1024;i++){
    buf[i] = (i+30)%255;
  }



}
