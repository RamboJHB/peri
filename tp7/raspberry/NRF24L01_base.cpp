#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/time.h>

#define MAXServerResquest 1024

typedef uint8_t byte;


using namespace std;

RF24 radio(15,8); // radio(CE,CS)

byte addresses[][6] = {"0iang","1iang","2iang"};

void setup() {
  radio.begin();
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1,addresses[0]);
  radio.startListening();
  radio.printDetails();
}

void loop_receive(FILE *fp, int val, int* line) {


}

int main(int argc, char **argv)
{
  FILE *fp;
  time_t t;
  int val = 0,line=1 ;
  char buf[64];
  char time_str[128];
  int  s2f;                               // fifo file descriptors
  char    serverRequest[MAXServerResquest];               // buffer for the request
  fd_set  rfds;                                           // flag for select
  struct  timeval tv;                                     // timeout
  tv.tv_sec = 0;                                          // 
  tv.tv_usec = 100;                                         //

  mkfifo("/tmp/s2f_JL", 0666);                                  // fifo creation

  //fopen new float of file
  if((fp=fopen("./server/www/log_in","a+"))==NULL){
    perror("failed fopen");exit(-1);
  }
  if ((s2f = open("/tmp/s2f_JL", O_RDWR))<0) {
	perror("s2f open failed");
	exit(1);
  }

  setup();

  do{
    t = time(0);
    t+=1500000500;
    //sscanf(ctime(&t),"%[^\n]",time_str);
	//cout<< time_str << endl;
    if( radio.available()){
      radio.read( &val, sizeof(int) ); // Get the payload
      fprintf(fp,"%u,%d\n",t,val);//write in file log_in//------------------------csv file
      fflush(fp);
      //cout << "received: ";cout <<val<<endl;// for printing val "\n" is obligate!!!! why????????????????????????
    }
    /*FD_ZERO(&rfds);                                     // erase all flags
    FD_SET(s2f, &rfds);                                 // wait for s2f
    FD_SET(STDIN_FILENO, &rfds);                        // wait for stdin
    if (select(s2f+1, &rfds, NULL, NULL, &tv) != 0) {   // wait until timeout=100 us
        int nbchar;
        //read server to fake pip
        if (FD_ISSET(s2f, &rfds)) {                     // something to read
            if ((nbchar = read(s2f, serverRequest, MAXServerResquest)) == 0) break;
            serverRequest[nbchar]=0;
            fprintf(stderr,"mesg from sever : %s \n", serverRequest);
	        memset(serverRequest, 0, sizeof(serverRequest) );
        }
    }*/
  }while(1);
  fclose(fp);
  return 0;
}
