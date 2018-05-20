
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../ftd2xx.h"

#include <unistd.h>


FT_HANDLE ftHandle;
UCHAR BitMode;
FT_STATUS ftStatus;

DWORD BytesWritten;
byte TxBuffer[1];
//char TxBuffer[1];

// 22 RX LED
// 23 TX LED
// 2 DTR Bit4
// 5 RX  Bit1
// 1 TX  Bit0

void init_usb(void){
    FT_Open(0, &ftHandle);
    FT_SetBaudRate(ftHandle, 9600);
    
}//init_usb
void print_IO_state(void){
    FT_GetBitMode(ftHandle, &BitMode);
    //system("cls");
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
    sleep(1000);
    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(1000);
}// blink data0
void blink_data1(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x02;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(1000);
    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(1000);
}// blink data1
void blink_data4(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x10;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(1000);
    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &BytesWritten);
    print_IO_state();
    sleep(1000);
}// blink data4
void blink_CBUS0(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x20);
    print_IO_state();
    sleep(100);
    
    FT_SetBitMode(ftHandle, 0xFE, 0x20);
    print_IO_state();
    sleep(100);
    
}//CBUS0
void blink_CBUS1(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x20);
    print_IO_state();
    sleep(100);
    
    FT_SetBitMode(ftHandle, 0xFD, 0x20);
    print_IO_state();
    sleep(100);
    
}//CBUS1
void all_data_inputs(void){
    FT_SetBitMode(ftHandle, 0x00, 0x01);
    print_IO_state();
    sleep(100);
}//all_data_inputs
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
        blink_CBUS0();//LED1
        blink_CBUS1();//LED2
        blink_data0();//TX
        blink_data1();//RX
        //blink_data4();//DTR
        //all_data_inputs();//TX_RX_DTR
        //TXin_RXout();//TX is input RX is output
        
    }//while 1
    
}// main
