//http://dlacko.blogspot.com.es/2016/03/recycling-toy-model-ir-transmitters-silverlit.html
#include <IRremote.h>

int recvPin = 52;
IRrecv irrecv(recvPin);

void  setup ( )
{
  Serial.begin(9600);
  Serial.println("hol");
  irrecv.enableIRIn();  // Start the receiver
}

unsigned long decodeNano(decode_results *results)
{
  unsigned long value = 0;
  
  // Start at 2, skip header
  for (int i = 2;  i < results->rawlen;  i++) {

    // Skip even indexes
    if (i & 1) {
      int t = results->rawbuf[i] * USECPERTICK;
      value <<= 1;
      value += t > 500;
    }
  }

  return value;
}

void  loop ( )
{
  decode_results results; // Somewhere to store the results

  if (irrecv.decode(&results)) { // Grab an IR code

     Serial.println(decodeNano(&results));
  }

  irrecv.resume(); // Prepare for the next value
}
