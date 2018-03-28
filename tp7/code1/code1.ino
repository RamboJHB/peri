#include <gfxfont.h>
#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

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
#include <SPI.h>
#include <Wire.h>
#include "RF24.h"
#include "printf.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define MAX_WAIT_FOR_TIMER 3
#define OLED_RESET 4

RF24 radio(9,10);
byte addresses[][6] = {"0Node","1Node"};

Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

static int lin = 0;
static int len = 0;
char str[20];
static int ledc = 0;
static int rd_ms = 0;
int cap_var;
int erreur_tran = 0;

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
  B00000000, B00110000
};

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

enum etat {EMPTY, FULL};

struct mailbox {
  etat state;
  char val;
} mb0 = {.state = EMPTY};

void T1(struct mailbox *mb) {
  if (mb->state != EMPTY) return; // attend que la mailbox soit vide
  radio.read( &(mb->val), sizeof(char));             // Get the payload
  Serial.print(mb->val);
  mb->state = FULL;
}

void T2(struct mailbox *mb) {
  if (mb->state != FULL) return; // attend que la mailbox soit pleine
  
  if ((mb->val == 'A') && (rd_ms == 0)) {
    ledc = 1;
  }
  else if ((mb->val == 'E') && (rd_ms == 0)) {
    ledc = 0;
  }
  else if ((mb->val == 'P') && (rd_ms == 0)) {
    rd_ms = 1;
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
  }
  else if (rd_ms == 1) {
    if (mb->val == ' ') {
      rd_ms = 0;
      str[len] = '\0';
      display.print(str);
      display.display();
      len = 0;
    }
    else if(((mb->val>='a') && (mb->val <= 'z')) || ((mb->val>='A') && (mb->val <= 'Z')) || ((mb->val>='0') && (mb->val <= '9'))){
      str[len] = mb->val;
      ++len;
      /*display.print(&(mb->val));
      display.print(&(mb->val));
      display.display();*/
    }
  }

  mb->state = EMPTY;
}


unsigned int waitFor(int timer, unsigned long period) {
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];
  unsigned long newTime = micros() / period;              // numéro de la période modulo 2^32
  int delta = newTime - waitForTimer[timer];              // delta entre la période courante et celle enregistrée
  if ( delta < 0 ) delta += 1 + (0xFFFFFFFF / period);    // en cas de dépassement du nombre de périodes possibles sur 2^32
  if ( delta ) waitForTimer[timer] = newTime;             // enregistrement du nouveau numéro de période
  return delta;
}

void Led(int timer, long period, byte led) {
  static byte val = 0;                                    // valeur à mettre sur la led
  if (!waitFor(timer, period)) return;                   // sort s'il y a moins d'une période écoulée

  Serial.print(ledc);
  if (ledc == 1) {
    Serial.print(val);
    digitalWrite(led, val);                                 // ecriture
    delay(100);
    val = 1 - val;
  }                                          // changement d'état
  if (ledc == 0){
    digitalWrite(led, 0);
  }
  
}

void Getcap(int timer, long period){
  if (!(waitFor(timer, period)) && erreur_tran == 0) return;

  cap_var = analogRead(A0);
   Serial.print("   ");
  Serial.print(cap_var);
   Serial.print("   ");
  delay(100);
  radio.stopListening();
  if(!radio.write( &cap_var, sizeof(int))){
             Serial.print("error");
             erreur_tran = 1;
   }
   else{
             erreur_tran = 0;
    }
  delay(500);
  radio.startListening();
}

void Mess(int timer, long period, const char * mess) {
  if (!(waitFor(timer, period))) return;                   // sort s'il y a moins d'une période écoulée
  //display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  if (lin == 0) lin = 1;
  else  lin = 0;
  //display.setCursor(0,0);
  display.println(mess);
  display.display();                                   // affichage du message
}

void setup() {
  pinMode(13, OUTPUT);        // initialisation de la direction de la broche
  digitalWrite(13, 1);
  Serial.begin(115200);                                   // initialisation du débit de la liaison série
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1,addresses[0]);
  radio.openWritingPipe(addresses[1]);
  radio.printDetails();
  radio.startListening();
}

void loop() {
  unsigned long got_time;

  if( radio.available()){
     T1(&mb0);
     T2(&mb0);
   }

  Getcap(2, 2000000);
  Led (0, 500000, 13);                                    // Led est exécutée toutes les 100ms
  //if(lin == 0)
 //   Mess(1,2000000,"bonjour");                              // Mess est exécutée toutes les secondes
  //else
   // Mess(1,2000000,"jourbon");
}

