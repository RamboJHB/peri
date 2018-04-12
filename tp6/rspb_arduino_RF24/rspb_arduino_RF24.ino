//-----------------------OLED lib----------------------------
#include <SPI.h>
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
byte addresses[][6] = {"0XXXX"};

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
unsigned int cpt = 0;

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
 *  inter-tâches
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
  sprintf(mb->mess, "...");
}
void setup_oled() {
  Serial.begin(9600);                                     // initialisation du débit de la liaison série
  
  //----------------------------------------init OLED (i2c)------------------------------------------
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  display.clearDisplay();
}
  
void setup_led() {
  pinMode(13,OUTPUT);
}

void loop_read(struct mailbox_st* mb) {
  if ( !(waitFor(3,1500000))) return;
  if (mb->stt != EMPTY) return; // attend que la mailbox soit vide
  cpt++;
  mb->val =analogRead(15);        //----------------------------read capturer lumiere PIN15
  mb->stt = FULL;
}

void loop_display(struct mailbox_st* mb) {
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
  mb->stt = EMPTY;
}

static bool flag;

void loop_LED(struct mailbox_st* mb) {
  long int period_led = 100000000/(mb->val);
  //blink in diff frequency
  //Serial.println(period_led);
  if (!(waitFor(4,period_led))) return;
  if (not flag){
    digitalWrite(13, HIGH);flag=true;
  }else if(flag){ 
    digitalWrite(13, LOW);flag =false;
  }              
}
void setup_RF24() {
  radio.begin();
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1,addresses[0]);
  radio.printDetails();
  radio.startListening();
  Serial.begin(9600);
}
//--------- Setup et Loop---------------------------------------------------//
//--------- Déclaration des tâches

struct mailbox_st mb0;

void setup() {
  setup_mailbox(&mb0,0);                       
  setup_oled();          
  //setup_led();
  setup_RF24(); 
}

void loop() {
  char buffer[32];
  //loop_LED(&mb0);//------------------------------------problem here : LED blink frequency how to fix
  loop_read(&mb0);                                        
  
  if( radio.available()){
    radio.read( buffer, sizeof(buffer) ); // Get the payload
    Serial.println(buffer);
    sprintf(mb0.mess,"lalala");
  }else{  
    loop_display(&mb0);
  }
}
