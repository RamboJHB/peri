#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftdi.h>

#define BIT(b,n)   (((n)>>(b))&1) // BIT(3, 0x000100) return 1
#define TXD 0x04  /* TXD (orange wire on FTDI cable) */

volatile unsigned char pan       = 85;   //0-255
volatile unsigned char tilt      = 85;  //0-255
volatile unsigned char ctrl      = 85; //0-7 008-134
volatile unsigned char red       = 85;
volatile unsigned char green     = 85;
volatile unsigned char blue      = 85;
volatile unsigned char white     = 85;
volatile unsigned char speed     = 85;
//volatile unsigned char reset     = 85;

unsigned char frame[1];

const unsigned char one    = 1;
const unsigned char zero   = 0;


void ftdi_fatal (struct ftdi_context *ftdi, char *str){
    fprintf (stderr, "%s: %s\n",
             str, ftdi_get_error_string (ftdi));
    ftdi_free(ftdi);
    exit (1);
}

void send_one(struct ftdi_context *ftdi, int delay){
  if (ftdi_write_data(ftdi, &zero, 1) < 0){
  //if (ftdi_setdtr(ftdi, 1) < 0){
        fprintf(stderr,"write failed for 0x%x: %s\n",
                one, ftdi_get_error_string(ftdi));
    }
    usleep(1);
}

void send_zero(struct ftdi_context *ftdi, int delay){
    if (ftdi_write_data(ftdi, &zero, 1) < 0){
    //if (ftdi_setdtr(ftdi, 0)){
        fprintf(stderr,"write failed for 0x%x: %s\n",
                zero, ftdi_get_error_string(ftdi));
    }
    usleep(1);
}
void send_frame(struct ftdi_context *ftdi, int delay, int frame){
    //start bit
    send_zero(ftdi, delay);

    int i ;
    for (i = 0; i < 8 ; i++){
        if (BIT(i,frame) == 1){
            send_one(ftdi, delay);
        }else if(BIT(i,red) == 0){
            send_zero(ftdi, delay);
        }
    }
    //2 stop bits
    send_one(ftdi, delay);
    send_one(ftdi, delay);

}

void send(struct ftdi_context *ftdi, int delay){

    int i;
    //printf("\nsending MTBF...");
    send_one(ftdi, delay);
    usleep(1800);//Mark Time Between Frames

    //printf("\nbreak...");
    //send_zero(ftdi, delay);
    usleep(176);//break

    //printf("\nsending MAB...");
    //send_one(ftdi, delay);
    usleep(88);//Mark-After-Break

    //printf("\nsending data splot 0...\n");
    for (i = 0; i < 9; i++)
        send_zero(ftdi, delay);

    send_one(ftdi, delay);
    send_one(ftdi, delay);

    //printf("\nbegin sending data...\n");
    //send_frame(ftdi ,delay ,pan);
    //send_frame(ftdi ,delay ,tilt);
    //send_frame(ftdi ,delay ,ctrl);
    send_frame(ftdi ,delay ,red);
    //send_frame(ftdi ,delay ,green);
    //send_frame(ftdi ,delay ,blue);
    //send_frame(ftdi ,delay ,white);
    //send_frame(ftdi ,delay ,speed);
    //send_frame(ftdi ,delay ,reset);
    //printf("\n");
}
void test_send1(struct ftdi_context *ftdi){
  unsigned char b = 1;
  unsigned char a = 0;

  while(1){
    if (ftdi_write_data(ftdi, &a, sizeof(a)) < 0){
          fprintf(stderr,"write failed for 0x%x: %s\n",
                  a, ftdi_get_error_string(ftdi));
    }
    usleep(1);
    if (ftdi_write_data(ftdi, &b, sizeof(b)) < 0){
          fprintf(stderr,"write failed for 0x%x: %s\n", b, ftdi_get_error_string(ftdi));
    }
    usleep(1);
  }
}
void test_send2(struct ftdi_context *ftdi){
  frame[0]=0b01010101;
  while(1){
    ftdi_write_data(ftdi, frame, 1);
  }
}

void test_send4(struct ftdi_context *ftdi, int pattern, int baudrate){

  if (ftdi_set_line_property(ftdi, 8, STOP_BIT_2, NONE) < 0)
      ftdi_fatal(ftdi, "unable to set line parameters");
  unsigned char buf[2];
  buf[0]=pattern;
  buf[1]=pattern;
  while (1)
    ftdi_write_data(ftdi, buf,
                      (baudrate/512 > sizeof(buf))?sizeof(buf):
                      (baudrate/512)?baudrate/512:1);

}



void test_bit(int frame){
    int i ;
    for (i = 0; i < 8 ; i++){
        if (BIT(i,frame) == 1){
            printf("1");
        }else if(BIT(i,red) == 0){
            printf("0");
        }
    }
}


int main(int argc, char **argv){

    struct ftdi_context *ftdi;
    int baudrate = 230400;
    int t;
    int delay = 2; /* 2 microseconds: 1 tenth of a second */
    unsigned int pattern = 0xf0f0;

    while ((t = getopt (argc, argv, "d:")) != -1)
    {
        switch (t)
        {
            case 'd':
                delay = atoi (optarg);
                break;
        }
    }

    if ((ftdi = ftdi_new()) == 0)
    {
        fprintf(stderr, "ftdi_new failed\n");
        return EXIT_FAILURE;
    }

    if (ftdi_usb_open(ftdi, 0x0403, 0x6001) < 0)
        ftdi_fatal (ftdi, "Can't open ftdi device");

    if (ftdi_set_baudrate(ftdi, baudrate) < 0)
        ftdi_fatal (ftdi, "Can't set baudrate");




    //test_send1(ftdi);
    //test_send2(ftdi);
    //test_send3(ftdi);
    //test_send4(ftdi, pattern,baudrate);

    bool flag_r;
    if (ftdi_set_bitmode(ftdi, 0xFF, BITMODE_BITBANG) < 0)
        ftdi_fatal (ftdi, "Can't enable bitbang");
    while (1) {
        if (red == 255 )    flag_r = true;
        if (red == 0 )      flag_r = false;
        if (flag_r) red ++;
        else        red --;
        send(ftdi, delay);
    }

    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
    exit (0);
}
