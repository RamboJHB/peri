//-----------------------OLED lib----------------------------
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

//-----------------------RF24 lib----------------------------
#include <SPI.h>
#include "RF24.h"
#include "printf.h"


RF24 radio(9,10);

byte addresses[][6] = {"0iang"};

//-----------------------OLED set up----------------------------
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//-----------------------------vals-----------------------------

/********************************************************
 *********************************************************
 *  timer  period = 1000000 ---> 1s delay
**********************************************************
*********************************************************/

#define MAX_WAIT_FOR_TIMER 5

//waitForTimer[MAX_WAIT_FOR_TIMER] = number of periods (from beginning of the prog) of last call waitFor()
//beginning
 
unsigned int waitFor(int timer, unsigned long period){
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];
  
  //micros()  Returns the number of microseconds since the Arduino board began running the current program.
  //This number will overflow (go back to zero), after approximately 70 minutes.
  unsigned long newTime = micros() / period;              // numéro de la période modulo 2^32
  int delta = newTime - waitForTimer[timer];              // delta entre la période courante et celle enregistrée
  if ( delta < 0 ) delta += 1 + (0xFFFFFFFF / period);    // en cas de dépassement du nombre de périodes possibles sur 2^32 
  if ( delta ) waitForTimer[timer] = newTime;             // enregistrement du nouveau numéro de période
  return delta;
}

/********************************************************
 *********************************************************
 *  functions
**********************************************************
*********************************************************/
enum state  {EMPTY, FULL};
 
struct mailbox_st {
  state stt;
  int val;
  char mess[32];
};

void setup_mailbox(struct mailbox_st* mb ,int val) {
  mb->val = val;
  mb->stt = EMPTY;
  sprintf(mb->mess, "sending");
}
void setup_RF24(){
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(addresses[0]);
  Serial.println("ici");
  radio.printDetails();
}
void setup_oled() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  display.clearDisplay();
}
  
void loop_cap(struct mailbox_st* mb) {
  if ( !(waitFor(3,1500000))) return;
  if (mb->stt != EMPTY) return; // attend que la mailbox soit vide
  mb->val =analogRead(15);        //----------------------------read capturer lumiere PIN15
  //radio.write( &mb->val, sizeof(mb->val) ); // send to raspb
  mb->stt = FULL;
}

void loop_send_dis(struct mailbox_st* mb){
  if (mb->stt != FULL) return; // attend que la mailbox soit pleine
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print("Arduino:");
  
  display.setTextColor(WHITE);
  display.print(" "); display.print(mb->val);
  
  display.setCursor(0,10);
  display.setTextColor(BLACK, WHITE);
  display.print("Raspb:");
  
  display.setTextColor(WHITE);
  display.print(" "); display.print(mb->mess);
  
  display.display();
    if (!radio.write( &mb->val, sizeof(int) )){
      Serial.println(F("failed."));  //pass flash-memory based strings to Serial.print() by wrapping them with F()
  }else{
      Serial.print(F("sent "));
  }
  mb->stt = EMPTY;
}
/********************************************************
 *********************************************************
 *  setup()&loop
**********************************************************
*********************************************************/
struct mailbox_st mb0;

void setup(){
  setup_mailbox(&mb0,0);                       
  setup_RF24();// Setup and configure radio
  setup_oled();
}

void loop(void) {
  loop_cap(&mb0);
  loop_send_dis(&mb0);                             
} 
