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

extern char data[]="Hello_____";

void setup() {

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  
  radio.setRetries(15,15);

  radio.stopListening();
  
  radio.openWritingPipe(addresses[0]);
  
  radio.printDetails();
  
  delay(100);

}


void loop() {

  Serial.println(F("Now sending !"));

  if (!radio.write( &data, sizeof(data) )){
     Serial.println(F("failed!"));
  }else 
     Serial.println(F(data));

  delay(1000);

}

int main(int argc, char** argv){
    setup();
    while (1) loop();
    return 0;
}
