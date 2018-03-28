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
#define RS 7
#define E  27
#define D4 22
#define D5 23
#define D6 24
#define D7 25

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define RPI_BLOCK_SIZE  0xB4
#define RPI_GPIO_BASE   0x20200000

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

/*******************************************************************************
 * LCD's Instructions ( source = doc )
 ******************************************************************************/

#define B_0000  0
#define B_0001  1
#define B_0010  2
#define B_0011  3
#define B_0100  4
#define B_0101  5
#define B_0110  6
#define B_0111  7
#define B_1000  8
#define B_1001  9
#define B_1010  a
#define B_1011  b
#define B_1100  c
#define B_1101  d
#define B_1110  e
#define B_1111  f

/* On veut definir une macro HEX()
 * qui prend en argument deux nombres de 4 bits exprimes en binaire 
 * representant resp. les 4 bits de poids fort et les 4 bits de poids faible d'un entier sur 8bits  
 * qui rend l'entier.
 * p. ex: HEX(0000,0010) return 2
 * p. ex: HEX(0001,0000) return 16
 *
 * QUESTION: expliquez comment ca marche ....------------------------------------
 */
#define __HEX(h,l) 0x##h##l		//__HEX(3,c)==0x3c
#define _HEX(h,l)  __HEX(h,l)		// _HEX(3,c)==0x3c
#define HEX(h,l)   _HEX(B_##h,B_##l)	//  HEX(0011,1100)==_HEX(B_0011,B_1100)==_HEX(3,c)

/* QUESTION: que fait cette macro ? ----------------------------------return N°b bit of data 
 *  */
#define BIT(b,n)   (((n)>>(b))&1)

/* cursor */
static const DDRAM[4] = { 0, 0x40, 0x14, 0x54 };
#define CUR_SET(l,c) (0x80+DDRAM[(l)%4]+(c)%0x14)

/* commands */
#define LCD_CLEARDISPLAY        HEX(0000,0001)
#define LCD_RETURNHOME          HEX(0000,0010)
#define LCD_ENTRYMODESET        HEX(0000,0100)
#define LCD_DISPLAYCONTROL      HEX(0000,1000)
#define LCD_CURSORSHIFT         HEX(0001,0000)
#define LCD_FUNCTIONSET         HEX(0010,0000)
#define LCD_SETCGRAMADDR        HEX(0100,0000)
#define LCD_SETDDRAMADDR        HEX(1000,0000)

/* flags for display entry mode : combine with LCD_ENTRYMODESET */
#define LCD_EM_RIGHT            HEX(0000,0000)//----------------------------cursor 
#define LCD_EM_LEFT             HEX(0000,0010)
#define LCD_EM_DISPLAYSHIFT     HEX(0000,0001)
#define LCD_EM_DISPLAYNOSHIFT   HEX(0000,0000)

/* flags for display on/off control : combine with LCD_DISPLAYCONTROL */
#define LCD_DC_DISPLAYON        HEX(0000,0100)
#define LCD_DC_DISPLAYOFF       HEX(0000,0000)
#define LCD_DC_CURSORON         HEX(0000,0010)
#define LCD_DC_CURSOROFF        HEX(0000,0000)
#define LCD_DC_BLINKON          HEX(0000,0001)
#define LCD_DC_BLINKOFF         HEX(0000,0000)

/* flags for display/cursor shift : combine with LCD_CURSORSHIFT */
#define LCD_CS_DISPLAYMOVE      HEX(0000,1000)
#define LCD_CS_CURSORMOVE       HEX(0000,0000)
#define LCD_CS_MOVERIGHT        HEX(0000,0100)
#define LCD_CS_MOVELEFT         HEX(0000,0000)

/* flags for function set : combine with LCD_FUNCTIONSET */
#define LCD_FS_8BITMODE         HEX(0001,0000)//------------8bits/transation
#define LCD_FS_4BITMODE         HEX(0000,0000)
#define LCD_FS_2LINE            HEX(0000,1000)
#define LCD_FS_1LINE            HEX(0000,0000)
#define LCD_FS_5x10DOTS         HEX(0000,0100)
#define LCD_FS_5x8DOTS          HEX(0000,0000)//------------font size

/*******************************************************************************
 * LCD's Operations
 ******************************************************************************/

/* generate E signal --------------------give a falling edge*/
void lcd_strobe(void){
  gpio_update(E, 1);
  usleep(1);
  gpio_update(E, 0);
}

/* send 4bits to LCD */
void lcd_write4bits(int data){
  /* first 4 bits */
  gpio_update(D7, BIT(7,data));
  gpio_update(D6, BIT(6,data));
  gpio_update(D5, BIT(5,data));
  gpio_update(D4, BIT(4,data));
  lcd_strobe();//--------------------------------E falling edge to trigger Read and write
  /* second 4 bits */ 
  gpio_update(D7, BIT(3,data));
  gpio_update(D6, BIT(2,data));
  gpio_update(D5, BIT(1,data));
  gpio_update(D4, BIT(0,data));
  lcd_strobe();//--------------------------------E falling edge to trigger Read and write
  usleep(50); /* le délai minimum est de 37us */
}

void lcd_command(int cmd){
  gpio_update(RS, 0);
  lcd_write4bits(cmd);
  usleep(2000); /* delai nécessaire pour certaines commandes */
}

void lcd_data(int character){
  gpio_update(RS, 1);
  lcd_write4bits(character);//-------------no delay neccessary
}

/* initialization 
 * Question : commenter ces initialisations:
 *
 * LCD_FUNCTIONSET    | LCD_FS_4BITMODE | LCD_FS_2LINE | LCD_FS_5x8DOTS 
 * LCD_DISPLAYCONTROL | LCD_DC_DISPLAYON | LCD_DC_CURSOROFF 
 * LCD_ENTRYMODESET   | LCD_EM_RIGHT | LCD_EM_DISPLAYNOSHIFT 
 */
void lcd_init(void)
{
  gpio_update(E, 0);
  lcd_command(HEX(0011,0011)); /* initialization */ //--------DDRAM address
  lcd_command(HEX(0011,0010)); /* initialization */ //--------DDRAM address
  lcd_command(LCD_FUNCTIONSET    | LCD_FS_4BITMODE  | LCD_FS_2LINE | LCD_FS_5x8DOTS  );
  lcd_command(LCD_DISPLAYCONTROL | LCD_DC_DISPLAYON | LCD_DC_CURSORON | LCD_DC_BLINKON  );
  lcd_command(LCD_ENTRYMODESET   | LCD_EM_RIGHT     | LCD_EM_DISPLAYNOSHIFT);
}

void lcd_clear(void){
  lcd_command(LCD_CLEARDISPLAY);
  lcd_command(LCD_RETURNHOME);
}

void lcd_message(char* txt){ //--------------------one charactor = 8 bits 
  int i, j;
  for(i=0; i<20 && i<strlen(txt); i++){
    lcd_data(txt[i]);
  }
}
void lcd_set_cursor(int x, int y){
	x--;
	y--;
	lcd_command(CUR_SET(x,y));
}
/*******************************************************************************
 * Finally, the main function                 $sudo ./lcd_user.x "string"
 ******************************************************************************/
int main(int argc, char** argv)
{
  /* arg */
  if( argc < 2 ){
    fprintf(stderr, "ERROR: must take a string as argument\n");
    exit(1);
  }

  /* Retreive the mapped GPIO memory */
  if( gpio_setup() == -1 ){
    perror("ERROR: gpio_setup\n");
    exit(1);
  }
  printf("setting up gpio\n");
  /* Setting up GPIOs to output */
  gpio_config(RS, GPIO_OUTPUT);
  gpio_config(E , GPIO_OUTPUT);
  gpio_config(D4, GPIO_OUTPUT);
  gpio_config(D5, GPIO_OUTPUT);
  gpio_config(D6, GPIO_OUTPUT);
  gpio_config(D7, GPIO_OUTPUT);


  /* initialization */
  printf("setting up lcd\n");
  lcd_init();
  lcd_clear();

  /* print app argument */
 /* printf("sending %s\n",argv[1]);
  lcd_set_cursor(1,2);
  lcd_message(argv[1]);
  lcd_set_cursor(2,1);
  lcd_message(argv[2]);
  lcd_set_cursor(3,3);
  lcd_message(argv[3]);
  lcd_set_cursor(4,4);
  lcd_message(argv[4]);*/
  /*-------------------------print /prog/loadavg---------------------------*/
  char buf[64] ;
  int i =0;
  char *str;
  char *s = " \n";
  int fd = open ("/proc/loadavg", O_RDONLY );
  if ( fd < 0 ) {
	perror("ERROR: open file\n");
    return -1;
  }
   while (1){ 
		lcd_set_cursor(1,1);
		lcd_message("avg:");
		lcd_set_cursor(3,1);
		lcd_message("pid : ");
	        lcd_set_cursor(2,1);
		lcd_message("proc:");
		lseek(fd , 0, SEEK_SET);
		read(fd , buf ,64);
		
		str = strtok(buf , s);



		lcd_set_cursor(1,6);lcd_message(str);
		
		i =0;
		while (str != NULL ){
			if ( i == 1 ){
				lcd_set_cursor(1,11);lcd_message(str);
			}
			if ( i == 2 ){
				lcd_set_cursor(1,16);lcd_message(str);
			}
			if ( i == 3 ){
				lcd_set_cursor(2,6);lcd_message(str);
				}
			if ( i== 4 ) {
				lcd_set_cursor(3,6);lcd_message(str);
			}
			if ( i ==6 ) break;
			str = strtok(NULL, s);
			i++;
			
		}
		sleep(1);
		lcd_clear();

  }
  /* Release the GPIO memory mapping */
  gpio_teardown();

  return 0;
}
