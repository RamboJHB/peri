
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../ftd2xx.h"
#include <assert.h>

#include <unistd.h>

#define BIT(b,n)   (((n)>>(b))&1) // BIT(0x00100, 3) return 1
#define delay 1

FT_HANDLE ftHandle;
UCHAR BitMode;
FT_STATUS ftStatus;

DWORD BytesWritten;
UCHAR TxBuffer[1] ;

static UCHAR pan;   //0-255
static UCHAR tilt;  //0-255
static UCHAR ctrl; //0-7 008-134
static UCHAR red; //
static UCHAR green;
static UCHAR blue;
static UCHAR white;
static UCHAR speed;
static UCHAR reset;


// 22 RX LED
// 23 TX LED
// 2 DTR Bit4
// 5 RX  Bit1
// 1 TX  Bit0
/***********************************************************/

int init_usb(void){
    int portNum = 0;//0 port
    int baudRate = 9600 * 16; // baudrate
    
    printf("Trying FTDI device %d at %d baud.\n", portNum, baudRate);

    FT_Open(portNum, &ftHandle);
    ftStatus = FT_Open(portNum, &ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("FT_Open(%d) failed, with error %d.\n", portNum, (int)ftStatus);
        printf("Use lsmod to check if ftdi_sio (and usbserial) are present.\n");
        printf("If so, unload them using rmmod, as they conflict with ftd2xx.\n");
        return -1;
    }
    assert(ftHandle != NULL);
    
    ftStatus = FT_ResetDevice(ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_ResetDevice returned %d.\n", (int)ftStatus);
        return -1;
    }
    ftStatus = FT_SetBaudRate(ftHandle, (ULONG)baudRate);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_SetBaudRate(%d) returned %d.\n",
               baudRate,
               (int)ftStatus);
        return -1;
    }
    return 0;
    
}//init_usb


/***********************************************************/

void print_IO_state(void){
    FT_GetBitMode(ftHandle, &BitMode);
    printf ("Pins =0x%x | ", BitMode);
    printf ("TX pin %x \n", 1 & ~BitMode);
    //printf ("RX pin %x \n", (2 & ~BitMode)>>1);
    //printf ("DTR pin %x \n", (16 & ~BitMode)>>4);
}
void blink_TX(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x01;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(2);
    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(2);
    
}

void blink_CBUS0(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x20);
    print_IO_state();
    sleep(1);
    FT_SetBitMode(ftHandle, 0xFE, 0x20);
    print_IO_state();
    sleep(1);
}//CBUS0
void blink_CBUS1(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x20);
    print_IO_state();
    sleep(1);
    FT_SetBitMode(ftHandle, 0xFD, 0x20);
    print_IO_state();
    sleep(1);
}//CBUS1

/***********************************************************/

void generate_frame(){
    int i = 0;
    for (i = 0; i < 255 ; i++){
        red = (UCHAR)i;
    }
}

void send_one(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x01;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    
    
    FT_Write(ftHandle, TxBuffer, 1, &BytesWritten);
    print_IO_state();
}

void send_zero(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    FT_Write(ftHandle, TxBuffer, 1, &BytesWritten);
    print_IO_state();
}
void send(int frame){
    int i = 1;
    for (i = 1; i < 8 ; i++){
        if (BIT(frame,i)&1){
            send_one();
            
        }else{
            send_zero();
        }
    }
}
/***********************************************************/

int main(int argc, char *argv[]){
    generate_frame();
    send(0x0);
    if ( init_usb() != 0)
        return -1;
    
    while(1){
        //blink_CBUS0();//LED1
        //blink_CBUS1();//LED2
        //blink_TX();//TX
        
    }//while 1
    
}// main
