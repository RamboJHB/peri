#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ftd2xx.h"
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>


#define BIT(b,n)   (((n)>>(b))&1) // BIT(3, 0x000100) return 1


#define ARRAY_SIZE(x) sizeof((x))/sizeof((x)[0])



/* Test data which is easy to check visually */
static char testPattern[] = "1";
int        retCode = -1; // Assume failure
int        f = 0;
FT_STATUS  ftStatus = FT_OK;
FT_HANDLE  ftHandle = NULL;
int        portNum = -1; // Deliberately invalid
DWORD      bytesToWrite = 0;
DWORD      bytesWritten = 0;
int        inputRate = -1; // Entered on command line
int        baudRate = -1; // Rate to actually use
int        rates[] = {50, 75, 110, 134, 150, 200,
    300, 600, 1200, 1800, 2400, 4800,
    9600, 19200, 38400, 57600, 115200,
    230400, 460800, 576000, 921600};

/****************************************************************/

UCHAR TxBuffer[1] ;
UCHAR BitMode;
static UCHAR pan       = 0xA;   //0-255
static UCHAR tilt      = 0xA;  //0-255
static UCHAR ctrl      = 0xA; //0-7 008-134
static UCHAR red       = 0xA; //
static UCHAR green     = 0xA;
static UCHAR blue      = 0xA;
static UCHAR white     = 0xA;
static UCHAR speed     = 0xA;
static UCHAR reset     = 0xA;


// 22 RX LED
// 23 TX LED
// 2 DTR Bit4
// 5 RX  Bit1
// 1 TX  Bit0
/*********************************************************************************/


/***********************************************************/

void print_IO_state(void){
    FT_GetBitMode(ftHandle, &BitMode);
    printf ("Pins =0x%x | ", BitMode);
    printf ("TX pin %x \n", 1 & ~BitMode);
    printf ("RX pin %x \n", (2 & ~BitMode)>>1);
    printf ("DTR pin %x \n", (16 & ~BitMode)>>4);
}
void blink_TX(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x01;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    print_IO_state();
    sleep(2);
    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
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
    
}

void send_one(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x01;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    
    
    FT_Write(ftHandle, TxBuffer, 1, &bytesWritten);
    printf("1");
    //print_IO_state();
}

void send_zero(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    FT_Write(ftHandle, TxBuffer, 1, &bytesWritten);
    printf("0");
    
    //print_IO_state();
}
void send_data(UCHAR frame){
    send_zero();//start bit
    int i ;
    for (i = 0; i < 8 ; i++){
        if (BIT(i,frame) == 1){
            send_one();
        }else if(BIT(i,red) == 0){
            send_zero();
        }
    }
    
    send_one();send_one();//2 stop bits
}
void send(){
    int i;
    printf("\nsending MTBF...");
    send_one();     usleep(1800);//Mark Time Between Frames
    
    printf("\nbreak...");
    send_zero();    usleep(176);//break
    
    printf("\nsending MAB...");
    send_one();     usleep(88);//Mark-After-Break
    
    printf("\nsending data splot 0...\n");
    for (i = 0; i < 9; i++) send_zero();
    send_one();
    send_one();

    printf("\nbegin sending data...\n");
    //send_data(pan);
    //send_data(tilt);
    //send_data(ctrl);
    send_data(red);
    //send_data(green);
    //send_data(blue);
    //send_data(white);
    //send_data(speed);
    //send_data(reset);
    printf("\n");
    
}

/**********************************************************************************/
int main(int argc, char *argv[])
{
    bool flag_r = true;

    
    
    if (argc > 1)   sscanf(argv[1], "%d", &portNum);
    // Missing, or invalid.  Just use first port.
    if (portNum < 0)   portNum = 0;
    
    if (portNum > 16)
    {
        // User probably specified a baud rate without a port number
        printf("Syntax: %s [port number] [baud rate]\n", argv[0]);
        portNum = 0;
    }
    
    if (argc > 2)
    {
        sscanf(argv[2], "%d", &inputRate);
        
        for (f = 0; f < (int)(ARRAY_SIZE(rates)); f++)
        {
            if (inputRate == rates[f])
            {
                // User entered a rate we support, so we'll use it.
                baudRate = inputRate;
                break;
            }
        }
    }
    
    if (baudRate < 0)
        baudRate = 9600;
    
    printf("Trying FTDI device %d at %d baud.\n", portNum, baudRate);
    
    ftStatus = FT_Open(portNum, &ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("FT_Open(%d) failed, with error %d.\n", portNum, (int)ftStatus);
        printf("Use lsmod to check if ftdi_sio (and usbserial) are present.\n");
        printf("If so, unload them using rmmod, as they conflict with ftd2xx.\n");
        goto exit;
    }
    
    assert(ftHandle != NULL);
    
    ftStatus = FT_ResetDevice(ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_ResetDevice returned %d.\n", (int)ftStatus);
        goto exit;
    }
    
    ftStatus = FT_SetBaudRate(ftHandle, (ULONG)baudRate);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_SetBaudRate(%d) returned %d.\n",
               baudRate,
               (int)ftStatus);
        goto exit;
    }
    
    ftStatus = FT_SetDataCharacteristics(ftHandle,
                                         FT_BITS_8,
                                         FT_STOP_BITS_1,
                                         FT_PARITY_NONE);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_SetDataCharacteristics returned %d.\n", (int)ftStatus);
        goto exit;
    }
    
    // Indicate our presence to remote computer
    ftStatus = FT_SetDtr(ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_SetDtr returned %d.\n", (int)ftStatus);
        goto exit;
    }
    
    // Flow control is needed for higher baud rates
    ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0, 0);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_SetFlowControl returned %d.\n", (int)ftStatus);
        goto exit;
    }
    
    // Assert Request-To-Send to prepare remote computer
    ftStatus = FT_SetRts(ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_SetRts returned %d.\n", (int)ftStatus);
        goto exit;
    }
    
    ftStatus = FT_SetTimeouts(ftHandle, 3000, 3000);    // 3 seconds
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_SetTimeouts returned %d\n", (int)ftStatus);
        goto exit;
    }
    
    bytesToWrite = (DWORD)(sizeof(testPattern) - 1); // Don't write string terminator
    
    ftStatus = FT_Write(ftHandle,
                        testPattern,
                        bytesToWrite,
                        &bytesWritten);
    if (ftStatus != FT_OK)
    {
        printf("Failure.  FT_Write returned %d\n", (int)ftStatus);
        goto exit;
    }
    
    if (bytesWritten != bytesToWrite)
    {
        printf("Failure.  FT_Write wrote %d bytes instead of %d.\n",
               (int)bytesWritten,
               (int)bytesToWrite);
        goto exit;
    }
    
    /*******************************************************************/
    
    // bool flag_r;
    
    while (1) {
        if (red == 255 )    flag_r = false;
        if (red == 0 )      flag_r = true;
        if (flag_r) red ++;
        else        red --;
        send();
    }
    
    // Success
    retCode = 0;
    printf("Successfully wrote %d bytes\n", (int)bytesWritten);
    
exit:
    if (ftHandle != NULL)
        FT_Close(ftHandle);
    
    return retCode;
}


