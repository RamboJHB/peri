#include <SPI.h>
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10); // radio(CE,CS)

byte addresses[][6] = {"0Node"};

void setup() {
  Serial.begin(115200);
  printf_begin();
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.printDetails();
  delay(1000);
}


void loop() {
  Serial.println(F("Now sending !"));

  unsigned long start_time = millis();                             // Take the time, and send it.  This will block until complete
  if (!radio.write( &start_time, sizeof(unsigned long) )){
     Serial.println(F("failed!"));
  }
  
  delay(1000);
}
