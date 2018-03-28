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

int main (int argc,char **argv){
  FILE *fp;
  time_t t;
  int rand_val,line=1 ;
  char buf[64];
  char time_str[128];
  
  int  s2f;                               // fifo file descriptors


  char    serverRequest[MAXServerResquest];               // buffer for the request
  fd_set  rfds;                                           // flag for select
  struct  timeval tv;                                     // timeout
  tv.tv_sec = 1;                                          // 1 second
  tv.tv_usec = 0;                                         //

  mkfifo("/tmp/s2f_JL", 0666);                                  // fifo creation

  //fopen new float of file
  if((fp=fopen("../server/www/log_in","a+"))==NULL){
    perror("failed fopen");exit(-1);
  }
  if ((s2f = open("./s2f_JL", O_RDWR))<0) {
	perror("s2f open failed\n");
	exit(1);
  }                            // fifo openning
  while (fgets(buf,64,fp)!=0){        //count lines already existed
    if(buf[strlen(buf)-1]=='\n')
      line++;
  }

  do{
  
    time(&t);
    srandom(time(NULL));
    rand_val = (int)(100*(float)rand()/RAND_MAX) + 1000; 
    sscanf(ctime(&t),"%[^\n]",time_str);
    printf("%s,%d\n",time_str,rand_val);
    //strftime(time_sec, 128, "%S",localtime(&t));
    //fprintf(fp,"%s,%d\n",time_sec,rand_val+1000);
    fprintf(fp,"%d,%d\n",line++,rand_val);
    fflush(fp);
    
    FD_ZERO(&rfds);                                     // erase all flags
    FD_SET(s2f, &rfds);                                 // wait for s2f
    FD_SET(STDIN_FILENO, &rfds);                        // wait for stdin
    if (select(s2f+1, &rfds, NULL, NULL, &tv) != 0) {   // wait until timeout=1s
        int nbchar;
        //read server to fake pip
        if (FD_ISSET(s2f, &rfds)) {                     // something to read
            if ((nbchar = read(s2f, serverRequest, MAXServerResquest)) == 0) break;
            serverRequest[nbchar]=0;
            fprintf(stderr,"from sever ---------gate_way execution time = %s seconds\n", serverRequest);
	        memset(serverRequest, 0, sizeof(serverRequest) );
        }
    }
    sleep(1);
  }while(1);
  fclose(fp);
  return 0;
}

