/*******************************************************************************
 * lcdr_user.c - Controleur pour LCd HD44780 ( 20x4 )
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

/*******************************************************************************
 * GPIO Pins
 ******************************************************************************/


#define LED0   4
#define LED1   17
#define BP   18

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define RPI_BLOCK_SIZE  0xB4
#define RPI_GPIO_BASE   0x20200000

#define MAXServerResquest 1024

struct gpio_s
{
  uint32_t gpfsel[7];
  uint32_t gpset[3];
  uint32_t gpclr[3];
  uint32_t gplev[3];
  uint32_t gpeds[3];
  uint32_t gpren[3];
  uint32_t gpfen[3];
  uint32_t gphen[3];
  uint32_t gplen[3];
  uint32_t gparen[3];
  uint32_t gpafen[3];
  uint32_t gppud[1];
  uint32_t gppudclk[3];
  uint32_t test[1];
};
volatile struct gpio_s* gpio_regs;

/*******************************************************************************
 * GPIO Operations 
 ******************************************************************************/

int gpio_setup ( void ){

  int mmap_fd = open ("/dev/mem", O_RDWR | O_SYNC );
  if ( mmap_fd < 0 ) {
    return -1;
  }

  gpio_regs = mmap ( NULL, RPI_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mmap_fd, RPI_GPIO_BASE );
  if ( gpio_regs == MAP_FAILED ) {
    close ( mmap_fd );
    return -1;
  }

  return 0;
}

void  gpio_teardown ( void ){
  munmap((void*)gpio_regs, RPI_BLOCK_SIZE);
}


void gpio_config(int gpio, int value)
{
  gpio_regs->gpfsel[gpio/10] = ( gpio_regs->gpfsel[gpio/10] & ~(0x7 << ((gpio % 10) * 3)) )
                             | (value << ((gpio % 10) * 3));
}

void gpio_update(int gpio, int value)
{
  if( value )
    gpio_regs->gpset[gpio/32] = (0x1 << (gpio % 32) );
  else
    gpio_regs->gpclr[gpio/32] = (0x1 << (gpio % 32) );
}

int gpio_read(int pin){
     return ((gpio_regs->gplev[pin / 32]) >> (pin % 32)) & 0x1;  
}

/*******************************************************************************
 * GPIO Operations 
 ******************************************************************************/

int main ( int argc, char **argv )
{

  /* Retreive the mapped GPIO memory */
  if( gpio_setup() == -1 ){
    perror("ERROR: gpio_setup\n");
    exit(1);
  }

    printf("setting up gpio\n");
    /* Setting up GPIOs to output/input */
    gpio_config(LED0, GPIO_OUTPUT);
    gpio_config(LED1, GPIO_OUTPUT);
    gpio_config(BP, GPIO_INPUT);


    printf ( "-- info: start making pipe.\n" );

    /* ----------------------------------fake--------------------------------------- */
    int     f2s, s2f, max_fd;                               // fifo file descriptors
    char    *f2sName = "/tmp/f2s_JL";                       // filo names
    char    *s2fName = "/tmp/s2f_JL";                       //
    char    serverRequest[MAXServerResquest];               // buffer for the request
    fd_set  rfds;                                           // flag for select
    struct  timeval tv;                                     // timeout
    tv.tv_sec = 2;                                          // 1 second
    tv.tv_usec = 0;                                         //
    char buf[10];
    mkfifo(s2fName, 0666);                                  // fifo creation
    mkfifo(f2sName, 0666);

    /* open both fifos */
    if ((s2f = open(s2fName, O_RDWR))<0) {
	perror("s2f open failed\n");
	exit(1);
    }                            // fifo openning

    if ((f2s = open(f2sName, O_RDWR))<0) {
	perror("f2s open failed\n");
	exit(1);
    }                            // fifo openning

    do {
        FD_ZERO(&rfds);                                     // erase all flags
        FD_SET(s2f, &rfds);                                 // wait for s2f
        FD_SET(f2s, &rfds);                                 // wait for f2s
        FD_SET(STDIN_FILENO, &rfds);                        // wait for stdin
	
	max_fd = s2f > f2s ? s2f:f2s;
        if (select(max_fd+1, &rfds, NULL, NULL, &tv) != 0) {   // wait until timeout=1s
            int nbchar;

            /***read server to fake pip************************/
            if (FD_ISSET(s2f, &rfds)) {                     // something to read

                if ((nbchar = read(s2f, serverRequest, MAXServerResquest)) == 0) break;
                serverRequest[nbchar]=0;
                fprintf(stderr,"from server read %s", serverRequest);
		//**********LED APP*******
                if (serverRequest[0] == '0'){
		    gpio_update(LED1 , 0);
		    gpio_update(LED0 , 0);
		
		}                
		if (serverRequest[0] == '1'){
		    gpio_update(LED1 , 1);
		    gpio_update(LED0 , 0);
		
		}
		if (serverRequest[0] == '2'){
		    gpio_update(LED0 , 1);
		    gpio_update(LED1 , 0);
		}
		if (serverRequest[0] == '3'){
		    gpio_update(LED0 , 1);
		    gpio_update(LED1 , 1);
		}
		//**********SEND A RETURN TO SERVER******
		memset(serverRequest, 0, sizeof(serverRequest) );
		//char *enter = "\n";
                //if (( write(f2s, enter, sizeof(enter))) == 0) break;
            }
	
        }
	//*****************SEND BP TO SERVER*********//
    	int bp = (int) (gpio_read(BP));
	if (bp == 0){
	   	//printf("butt was pressed bp = %d\n", bp);
        	//if ( write(f2s, &bp, 1 ) == 0) break;
		//sleep(1);
	}
	if (FD_ISSET(f2s, &rfds)){
		write(f2s, &bp, 1 );
		if (bp == 0) {
			printf("bp = %d was sent to server\n",bp);
			//sleep(1);
		}
	} 
    }
    while (1);

    close(f2s);
    close(s2f);

    gpio_teardown;
    return 0;
}
