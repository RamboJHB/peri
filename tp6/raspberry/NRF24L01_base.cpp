#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <ctime>

typedef uint8_t byte;

using namespace std;

RF24 radio(15,8); // radio(CE,CS)

byte addresses[][6] = {"0XXXX"};

void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.printDetails();
}

char mess[16];
char buffer[32];
unsigned long timer;

void loop() {
  cout << "." << flush;
  sprintf(buffer,"%s %d", mess, timer);
  radio.write( buffer, sizeof(buffer) );
  timer++;  
  sleep(1);
}

int main(int argc, char **argv)
{
    if (argc > 1) strcpy(mess, argv[1]);
    setup();
    while (1)
        loop();
    return 0;
}
