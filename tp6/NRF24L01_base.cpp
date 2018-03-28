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

int fd = open("/dev/ledbp_XW", O_RDWR);
char bb;

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
  unsigned long get_info;

  if( radio.available()){
     radio.read( &get_info, sizeof(unsigned long) );             // Get the payload
     cout << get_info << endl;
     if(get_info == 20){
	bb = '1';
	write(fd,&bb,1);
     }
     else if(get_info == 10){
	bb = '0';
	write(fd,&bb,1);
     }
   }
}

int main(int argc, char** argv){
    setup();
    while (1) loop();
    return 0;
}
