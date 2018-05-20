/*
	Test app to send data to a terminal monitoring a second serial port.
 
    in terminal : $./write 2
*/

#include <stdio.h>
#include <assert.h>
#include "../ftd2xx.h"
#include <string.h>
#include <math.h>
#include "../ftd2xx.h"

#include <unistd.h>


#define ARRAY_SIZE(x) sizeof((x))/sizeof((x)[0])



/* Test data which is easy to check visually */
static int testPattern[] ={10,0,0};

//byte TxBuffer[1];
unsigned char TxBuffer[1];
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
UCHAR BitMode;

// 22 RX LED
// 23 TX LED
// 2 DTR Bit4
// 5 RX  Bit1
// 1 TX  Bit0

void init_usb(void){
    int portNum = 0;
    FT_Open(portNum, &ftHandle);//0 port
    FT_SetBaudRate(ftHandle, 9600);
    ftStatus = FT_Open(portNum, &ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("FT_Open(%d) failed, with error %d.\n", portNum, (int)ftStatus);
        printf("Use lsmod to check if ftdi_sio (and usbserial) are present.\n");
        printf("If so, unload them using rmmod, as they conflict with ftd2xx.\n");
    }
}//init_usb
void print_IO_state(void){
    FT_GetBitMode(ftHandle, &BitMode);
    printf ("Pins =0x%x \t", BitMode);
    printf ("TX pin %x \t", 1 & ~BitMode);
    //printf ("RX pin %x \t", (2 & ~BitMode)>>1);
    //printf ("DTR pin %x \t", (16 & ~BitMode)>>4);
    printf ("\n");

}
void blink_data0(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x01;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    print_IO_state();
    sleep(2);    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    print_IO_state();
    sleep(2);}// blink data0
void blink_data1(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x02;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    print_IO_state();
    sleep(2);    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    print_IO_state();
    sleep(2);}// blink data1
void blink_data4(void){
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    TxBuffer[0]= 0x10;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    print_IO_state();
    sleep(2);    TxBuffer[0]= 0x00;
    FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
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
        FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    }
    else{
        TxBuffer[0]= 0x00;
        FT_Write(ftHandle, TxBuffer, sizeof(TxBuffer), &bytesWritten);
    }
    
    sleep(10);
}//TXin_RXout


int main(int argc, char *argv[])
{
    /********************* OPEN USB DEVICE*********************/
	if (argc > 1)
	{
		sscanf(argv[1], "%d", &portNum);
	}
	
	if (portNum < 0)
	{
		// Missing, or invalid.  Just use first port.
		portNum = 0;
	}
	
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
    /**************************** RESET ******************************/
	assert(ftHandle != NULL);

	ftStatus = FT_ResetDevice(ftHandle);
	if (ftStatus != FT_OK) 
	{
		printf("Failure.  FT_ResetDevice returned %d.\n", (int)ftStatus);
		goto exit;
	}
    /**************************** SET BAUD RATE ******************************/
	ftStatus = FT_SetBaudRate(ftHandle, (ULONG)baudRate);
	if (ftStatus != FT_OK) 
	{
		printf("Failure.  FT_SetBaudRate(%d) returned %d.\n", 
		       baudRate,
		       (int)ftStatus);
		goto exit;
	}
	
    /************************* Set Data Characteristics *************************/
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

	ftStatus = FT_SetTimeouts(ftHandle, 3000, 3000);	// 3 seconds
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

	// Success
	retCode = 0;
	printf("Successfully wrote %d bytes\n", (int)bytesWritten);

exit:
	if (ftHandle != NULL)
		FT_Close(ftHandle);

	return retCode;
}
