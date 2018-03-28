#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

typedef uint8_t byte;

using namespace std;

RF24 radio(15,0);

byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};

void setup() {

  pinMode(13 , OUTPUT);
  delay(100);

  radio.begin();
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1,addresses[0]);
  radio.printDetails();
  radio.startListening();
}

void loop() {
  unsigned long got_time;
  //blink
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);


  if( radio.available()){
     radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
     cout << got_time << endl;
   }
}

int main(int argc, char** argv){
    setup();
    while (1) loop();
    return 0;
}
