
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../ftd2xx.h"
#include <assert.h>

#include <unistd.h>

#define BIT(b,n)   (((n)>>(b))&1) -- BIT(0x00100, 3) return 1
#define delay 1

FT_HANDLE ftHandle;
UCHAR BitMode;
FT_STATUS ftStatus;

DWORD BytesWritten;
unsigned char TxBuffer[1];

// 22 RX LED
// 23 TX LED
// 2 DTR Bit4
// 5 RX  Bit1
// 1 TX  Bit0

int init_usb(void){
    int portNum = 0;//0 port
    int baudRate = 9600; // Rate to actually use
    
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

void generate_frame{}
void send(int frame){
    int i = 1;
    for (i = 1; i < 100000 ; i++){
        if (BIT(frame,i)&1){
            send_one();

        }else{
            send_zero();
        }
        sleep(delay);
        usleep(delay);
    }
}
void send_one(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    FT_Write(ftHandle, 1, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
}

void send_zero(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    FT_Write(ftHandle, 0, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
}
void
void print_IO_state(void){
    FT_GetBitMode(ftHandle, &BitMode);
    printf ("Pins =0x%x \n", BitMode);
    printf ("TX pin %x \n", 1 & ~BitMode);
    printf ("RX pin %x \n", (2 & ~BitMode)>>1);
    printf ("DTR pin %x \n", (16 & ~BitMode)>>4);
}
void blink_data0(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x01;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(2);    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(2);}// blink data0
void blink_data1(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x02;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(2);    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(2);}// blink data1
void blink_data4(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x10;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(2);    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(2);}// blink data4
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
void all_data_inputs(void){
    FT_SetBitMode(ftHandle, 0x00, 0x01);
    print_IO_state();
    sleep(1);}//all_data_inputs
void TXin_RXout(void){
    FT_SetBitMode(ftHandle, 0x02, 0x01);
    print_IO_state();
    if(1 & ~BitMode){
        TxBuffer[0]= 0x02;
        FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    }
    else{
        TxBuffer[0]= 0x00;
        FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    }
    
    sleep(10);
}//TXin_RXout

int main(void)
{
    init_usb();
    
    while(1){
        //blink_CBUS0();//LED1
        //blink_CBUS1();//LED2
        //blink_data0();//TX
        //blink_data1();//RX
        //blink_data4();//DTR
        //all_data_inputs();//TX_RX_DTR
        //TXin_RXout();//TX is input RX is output
        
    }//while 1
    
}// main
