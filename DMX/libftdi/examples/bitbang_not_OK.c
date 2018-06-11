#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftdi.h>

#define BIT(b,n)   (((n)>>(b))&1) // BIT(3, 0x000100) return 1
#define TXD 0x04  /* TXD (orange wire on FTDI cable) */

//static int pan       = 85;   //0-255
//static int tilt      = 85;  //0-255
//static int ctrl      = 85; //0-7 008-134
volatile int red       = 85;
//static int green     = 85;
//static int blue      = 85;
//static int white     = 85;
//static int speed     = 85;
//static int reset     = 85;

const unsigned char one    = 1;
const unsigned char zero   = 0;

void send_one(struct ftdi_context *ftdi, int delay){

    if (ftdi_write_data(ftdi, &one, 1) < 0){
        fprintf(stderr,"write failed for 0x%x: %s\n",
                one, ftdi_get_error_string(ftdi));
    }
    usleep(delay);
}

void send_zero(struct ftdi_context *ftdi, int delay){

    if (ftdi_write_data(ftdi, &zero, 1) < 0)
    {
        fprintf(stderr,"write failed for 0x%x: %s\n",
                zero, ftdi_get_error_string(ftdi));
    }
    usleep(delay);
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
    printf("\nsending MTBF...");
    send_one(ftdi, delay);
    usleep(1800);//Mark Time Between Frames

    printf("\nbreak...");
    send_zero(ftdi, delay);
    usleep(176);//break

    printf("\nsending MAB...");
    send_one(ftdi, delay);
    usleep(88);//Mark-After-Break

    printf("\nsending data splot 0...\n");
    for (i = 0; i < 9; i++)
        send_zero(ftdi, delay);

    send_one(ftdi, delay);
    send_one(ftdi, delay);

    printf("\nbegin sending data...\n");
    //send_frame(ftdi ,delay ,pan);
    //send_frame(ftdi ,delay ,tilt);
    //send_frame(ftdi ,delay ,ctrl);
    send_frame(ftdi ,delay ,red);
    //send_frame(ftdi ,delay ,green);
    //send_frame(ftdi ,delay ,blue);
    //send_frame(ftdi ,delay ,white);
    //send_frame(ftdi ,delay ,speed);
    //send_frame(ftdi ,delay ,reset);
    printf("\n");

}
void test(int frame){
    int i ;
    for (i = 0; i < 8 ; i++){
        if (BIT(i,frame) == 1){
            printf("1");
        }else if(BIT(i,red) == 0){
            printf("0");
        }
    }

}
void ftdi_fatal (struct ftdi_context *ftdi, char *str)
{
    fprintf (stderr, "%s: %s\n",
             str, ftdi_get_error_string (ftdi));
    ftdi_free(ftdi);
    exit (1);
}

int main(int argc, char **argv){

    struct ftdi_context *ftdi;
    int t;
    int delay = 2; /* 2 microseconds: 1 tenth of a second */

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

    if (ftdi_set_baudrate(ftdi, 250000) < 0)
        ftdi_fatal (ftdi, "Can't set baudrate");

    if (ftdi_set_bitmode(ftdi, 0xFF, BITMODE_BITBANG) < 0)
        ftdi_fatal (ftdi, "Can't enable bitbang");


    bool flag_r;

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
