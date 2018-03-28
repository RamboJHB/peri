#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MAXServerResquest 1024


int main()
{
    int     f2s, s2f,maxfdp;                                       // fifo file descriptors
    char    *f2sName = "/tmp/f2s_fw";                       // filo names
    char    *s2fName = "/tmp/s2f_fw";                       //
    char    serverRequest[MAXServerResquest];               // buffer for the request
    fd_set  rfds;                                           // flag for select
    struct  timeval tv;                                     // timeout
    tv.tv_sec = 1;                                          // 1 second
    tv.tv_usec = 0;                                         //
    
    mkfifo(s2fName, 0666);                                  // fifo creation
    mkfifo(f2sName, 0666);
    
    char buf [10];
    
    /* open both fifos */
    s2f = open(s2fName, O_RDWR);                            // fifo openning
    f2s = open(f2sName, O_RDWR);
    
    do {
        FD_ZERO(&rfds);                                     // erase all flags
        FD_SET(s2f, &rfds);                                 // wait for s2f
        FD_SET(f2s, &rfds);                                 // wait for f2s
        FD_SET(STDIN_FILENO, &rfds);                        // wait for s2f
        maxfdp=s2f>f2s?s2f+1:f2s+1;
        if (select(s2f+f2s+1, &rfds, NULL, NULL, &tv) != 0) {   // wait until timeout
            int nbchar;
            if (FD_ISSET(s2f, &rfds)) {                     // something to read
                
                // read  s2f
                if ((nbchar = read(s2f, serverRequest, MAXServerResquest)) == 0) break;
                printf("---s2f---\n");
                serverRequest[nbchar]=0;
                fprintf(stderr,"%s", serverRequest);
            }
            
            if (FD_ISSET(STDIN_FILENO, &rfds)) {                     // something to read
                //read stdin to buf
                if ((nbchar = read(STDIN_FILENO, buf, sizeof(buf))) == 0) break;
                printf("---entered---\n");
                //write  buf to f2s
                if ((nbchar = write(f2s,buf, sizeof(buf))) == 0) break;
                printf("---f2s---\n");
                fprintf(stderr,"%s", serverRequest);
                memset (buf,0,sizeof(buf));
                //  serverRequest[nbchar]=0;
                //                write(f2s, serverRequest, nbchar);
            }
        }
    }while (1);
    
    close(f2s);
    close(s2f);
    
    return 0;
}
