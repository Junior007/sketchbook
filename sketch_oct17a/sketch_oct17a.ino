/*
  Calibration
 
 Demonstrates one technique for calibrating sensor input.  The
 sensor readings during the first five seconds of the sketch
 execution define the minimum and maximum of expected values
 attached to the sensor pin.
 
 The sensor minimum and maximum initial values may seem backwards.
 Initially, you set the minimum high and listen for anything 
 lower, saving it as the new minimum. Likewise, you set the
 maximum low and listen for anything higher as the new maximum.
 
 The circuit:
 * Analog sensor (potentiometer will do) attached to analog input 0
 * LED attached from digital pin 9 to ground
 
 created 29 Oct 2008
 By David A Mellis
 modified 30 Aug 2011
 By Tom Igoe
 
 http://arduino.cc/en/Tutorial/Calibration
 
 This example code is in the public domain.
 
 */

// These constants won't change:
const int luzPin = A0;    // pin that the sensor is attached to
const int ledPin = 9;        // pin that the LED is attached to

// variables:
long sensorValue = 0;         // the sensor value
long sensorMax = 1023;        // minimum sensor value
long sensorMin = 0;           // maximum sensor value


void setup() {
    // initialize serial communications at 9600 bps
  Serial.begin(9600);
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
/*
  // calibrate during the first five seconds 
  while (millis() < 5000) {
    sensorValue = analogRead(luzPin);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }
*/
  // signal the end of the calibration period
  digitalWrite(13, LOW);
}

void loop() {
  // read the sensor:
  sensorValue = analogRead(luzPin);

  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 1023);

  // in case the sensor value is outside the range seen during calibration
  /sensorValue = constrain(sensorValue, 0, 1023);

  // fade the LED using the calibrated value:
  //analogWrite(ledPin, sensorValue);
  delay(1000);
  Luminosidad();
  delay(1000);
}
void  Luminosidad(){
  sensorValue = (analogRead(luzPin));//I  
  Serial.print("R = ");//I  
  Serial.print(sensorValue);
  sensorValue = (sensorValue/1023.00)*100;//leer el valor del sensor de A0
  Serial.print("    L = ");
  Serial.print(sensorValue);//I  
  Serial.println("%");
}  
