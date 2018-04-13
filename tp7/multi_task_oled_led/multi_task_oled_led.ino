// --------------------------------------------------------------------------------------------------------------------
// Multi-tâches cooperatives : solution basique
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// unsigned int waitFor(timer, period) 
// Timer pour taches périodiques 
// configuration :
//  - MAX_WAIT_FOR_TIMER : nombre maximum de timers utilisés
// arguments :
//  - timer  : numéro de timer entre 0 et MAX_WAIT_FOR_TIMER-1
//  - period : période souhaitée
// retour :
//  - nombre de période écoulée depuis le dernier appel
// --------------------------------------------------------------------------------------------------------------------

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
 *  timer
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

//--------- définition de la tache Led

struct Led_st {
  int timer;                                              // numéro de timer utilisé par WaitFor
  unsigned long period;                                   // periode de clignotement
  int pin;                                                // numéro de la broche sur laquelle est la LED
  int etat;                                               // etat interne de la led
}; 

void setup_Led( struct Led_st * ctx, int timer, unsigned long period, byte pin) {
  ctx->timer = timer;
  ctx->period = period;
  ctx->pin = pin;
  ctx->etat = 0;
  pinMode(pin,OUTPUT);
  digitalWrite(pin, ctx->etat);
}

void loop_Led(struct Led_st * ctx) {
  if (!waitFor(ctx->timer, ctx->period)) return;          // sort s'il y a moins d'une période écoulée
  digitalWrite(ctx->pin,ctx->etat);                       // ecriture
  ctx->etat = 1 - ctx->etat;                              // changement d'état
}

//--------- definition de la tache Mess

struct Mess_st {
  int timer;                                              // numéro de timer utilisé par WaitFor
  unsigned long period;                                             // periode d'affichage
  char mess[20];
} Mess_t ; 

void setup_Mess(struct Mess_st * ctx, int timer, unsigned long period, const char * mess) {
  ctx->timer = timer;
  ctx->period = period;
  strcpy(ctx->mess, mess);
  Serial.begin(9600);                                     // initialisation du débit de la liaison série
  
  //----------------------------------------init OLED (i2c)------------------------------------------
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);
  display.clearDisplay();

  
}

void loop_Mess(struct Mess_st *ctx) {
  unsigned int t; 
  if (!(t = waitFor(ctx->timer,ctx->period))) return;         // sort s'il y a moins d'une période écoulée
  Serial.print(t);Serial.print(" ");
  Serial.println(ctx->mess);
   // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(ctx->mess);                              // affichage du message
  display.display();//!!!necessary
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
};

void setup_T1(struct mailbox_st* mb ,int val) {
  mb->val = val;
  mb->stt = EMPTY;
}
void setup_T2() {
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
  
void setup_T3(struct mailbox_st* mb) {
  pinMode(13,OUTPUT);
}

void loop_T1_sender(struct mailbox_st* mb) {
  if ( !(waitFor(3,1500000))) return;
  if (mb->stt != EMPTY) return; // attend que la mailbox soit vide
  cpt++;
  mb->val =analogRead(15);        //----------------------------read capturer lumiere PIN15
  mb->stt = FULL;
}

void loop_T2_reciever(struct mailbox_st* mb) {
  if (mb->stt != FULL) return; // attend que la mailbox soit pleine
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.print(mb->val); 
  display.display();
  mb->stt = EMPTY;
}
static bool flag;
void loop_T3_LED(struct mailbox_st* mb) {
  long int period_led = 100000000/(mb->val);

  Serial.println(period_led);
  if (!(waitFor(4,period_led))) return;
  if (not flag){
    digitalWrite(13, HIGH);flag=true;
  }else if(flag){ 
    digitalWrite(13, LOW);flag =false;
  }              
}

//--------- Setup et Loop---------------------------------------------------//
//--------- Déclaration des tâches

//struct Led_st Led1;
//struct Mess_st Mess1;
struct mailbox_st mb0;

void setup() {
  //period 1s = 1000000
  //setup_Led(&Led1, 0, 1500000, 13);                        // Led est exécutée toutes les 1s 
  //setup_Mess(&Mess1, 1, 2000000, "OMG WTF");              // Mess est exécutée toutes les 2,5 secondes 
  setup_T1(&mb0,0);                        // Led est exécutée toutes les 1s 
  setup_T2();              // Mess est exécutée toutes les 2,5 secondes 
  setup_T3(&mb0); 
}

void loop() {
  //loop_Led(&Led1);                                        
  //loop_Mess(&Mess1); 
  loop_T1_sender(&mb0);                                        
  loop_T2_reciever(&mb0);
  loop_T3_LED(&mb0);
}
