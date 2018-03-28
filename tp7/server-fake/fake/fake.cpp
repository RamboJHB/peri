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
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

typedef uint8_t byte;

using namespace std;
char start_info;

long i;
int cap_var;
time_t timep;

RF24 radio(15,0);

byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};

#define MAXServerResquest 1024

void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.printDetails();
  radio.startListening();
}

void loop() {
  
  
  if (!radio.write( &start_info, sizeof(char) )){
     cout << "error" << endl;
  }
  else
	cout << start_info << endl;

  sleep(1);
}

int main()
{
    setup();
    
    int     f2s, s2f, f2s2, s2f2;                                       // fifo file descriptors
    char    *f2sName = "/tmp/f2s_fw_xw1";                       // filo names
    char    *s2fName = "/tmp/s2f_fw_xw1";                       //
    char    *f2sName2 = "/tmp/f2s_fw_xw2";                       // filo names
    char    *s2fName2 = "/tmp/s2f_fw_xw2";                       //
    char    serverRequest[MAXServerResquest];               // buffer for the request
    fd_set  rfds,rfds2;                                           // flag for select
    struct  timeval tv;                                     // timeout
    tv.tv_sec = 1;                                          // 1 second
    tv.tv_usec = 0; 
    
    umask(0000);
    mkfifo(s2fName, 0666);                                  // fifo creation
    mkfifo(f2sName, 0666);
    mkfifo(s2fName2, 0666);                                  // fifo creation
    mkfifo(f2sName2, 0666);
    /* open both fifos */
    s2f = open(s2fName, O_RDWR);                            // fifo openning
    f2s = open(f2sName, O_RDWR);
    s2f2 = open(s2fName2, O_RDWR);                            // fifo openning
    f2s2 = open(f2sName2, O_RDWR);
    do {
        FD_ZERO(&rfds);                                     // erase all flags
        FD_SET(s2f, &rfds);                                 // wait for s2f
	FD_ZERO(&rfds2);                                     // erase all flags
        FD_SET(s2f2, &rfds2);                                 // wait for s2f
	if(radio.available()){
		char lin[6];
		int i = 0;
		int len;
		char *date;
		int mod_var = 1000;
		radio.read(&cap_var,sizeof(int));
		for(i = 0; i < 4; i ++){
			lin[i] = '0' + ((cap_var % (mod_var * 10))/ mod_var);
			mod_var = mod_var / 10;
		}
		lin[4] = '\n';
		lin[5] = '\0';
		write(f2s, lin, 6);
		cout << lin << endl;
		time(&timep);
		date = asctime(gmtime(&timep));
		len = strlen(date);
		write(f2s, date, len);
		cout << date << endl;
	}

	//LED
        if (select(s2f+1, &rfds, NULL, NULL, &tv) != 0) {   // wait until timeout
            if (FD_ISSET(s2f, &rfds)) {                     // something to read
                int nbchar;
		char a;
		char lin[3];
                if ((nbchar = read(s2f, serverRequest, MAXServerResquest)) == 0) break;
                serverRequest[nbchar]=0;
                //fprintf(stderr,"%s", serverRequest);
		cout << "aaa" << serverRequest[0] << endl;
		radio.stopListening();
		if((serverRequest[0]) == '1'){
			start_info = 'A';
			while (!radio.write( &start_info, sizeof(char))){
     				cout << "error" << endl;
				sleep(0.5);
  			}
  			cout << start_info << endl;
		}
		else if((serverRequest[0]) == '0'){
			start_info = 'E';
			while(!radio.write( &start_info, sizeof(char))){
     				cout << "error" << endl;
				sleep(0.5);
  			}
  			cout << start_info << endl;
		}
		sleep(0.5);
		radio.startListening();
		//loop();
            }
        }

	//receive message
	if (select(s2f2+1, &rfds2, NULL, NULL, &tv) != 0) {   // wait until timeout
            if (FD_ISSET(s2f2, &rfds2)) {                     // something to read
                int nbchar;
		char a;
		char lin[3];
                if ((nbchar = read(s2f2, serverRequest, MAXServerResquest)) == 0) break;
                serverRequest[nbchar]=0;
                //fprintf(stderr,"%s", serverRequest);
		start_info = 'P';
		radio.stopListening();
		while (!radio.write( &start_info, sizeof(char))){
     			cout << "error" << endl;
  		}
		cout << start_info << endl;
		sleep(0.5);
		cout << "aaa" << serverRequest << endl;
		
		for(i = 0; i < nbchar; i++){
			start_info = serverRequest[i];
			while(!radio.write( &start_info, sizeof(char))){
     				cout << "error" << endl;
  			}
 			cout << start_info << endl;
			sleep(0.5);
		}
		start_info = ' ';
		while(!radio.write( &start_info, sizeof(char))){
     			cout << "error" << endl;
  		}
		cout << start_info << endl;
		sleep(0.5);
  		radio.startListening();
            }
        }
	
    }
    while (1);

    close(f2s);
    close(s2f);
    close(f2s2);
    close(s2f2);

    return 0;
}
