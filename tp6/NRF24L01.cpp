#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef uint8_t byte;

using namespace std;
unsigned long start_info = 10;

long i;

RF24 radio(15,0);

byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};

void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.printDetails();
  
}

void loop() {
  
  
  if (!radio.write( &start_info, sizeof(unsigned long) )){
     cout << "error" << endl;
  }
  else
	cout << start_info << endl;

  sleep(1);

}

int main(int argc, char** argv){
    setup();
    radio.stopListening();
    while (1) {
        loop();
	if(start_info == 20)
		start_info = 10;
	else
		start_info = 20;
	}
    return 0;
}
