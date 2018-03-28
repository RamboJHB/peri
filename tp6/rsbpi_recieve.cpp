#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <string.h>

typedef uint8_t byte;

using namespace std;

RF24 radio(15,0);

byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};

void setup() {

  radio.begin();

  radio.setRetries(15,15);

  radio.setPALevel(RF24_PA_LOW);

  radio.openReadingPipe(1,addresses[0]);

  radio.printDetails();

  radio.startListening();
}

void loop() {
  
  unsigned long got_time;s

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
