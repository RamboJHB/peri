
Compte Rendu PERI TP6:Programmation Arduino
===========================================

Dans cette séance on a le but de faire la programmation sur Arduino. On a étudié que un programme Arduino est composé au minimum de 2 fonctions:

- setup() exécutée une fois pour initialiser les composants et les variables.

- loop() exécutée en boucle jusqu'à l'extinction de la carte.

Parce qu'il n'y a pas de système d'exploitation sur la carte Arduino, lorsqu'on veut expérimenter la programmation "multi-tâches",les "tâches" seront programmées de sorte à être indépendantes de leur contexte d'utilisation et elles seront "ordonnancées" par la fonction loop(). La fonction loop() demande donc l'exécution des tâches à tour de rôle. Les tâches n'ont pas le droit de conserver le processeur sinon cela crée un blocage du système.

## Question:

**Que contient le tableau waitForTimer[]?**

Le numéro de période enregistré. (en microseconde)

**Dans quel cas la fonction waitFor() peut rendre 2 ?**

La fonction waitFor() rend le nombre de période écoulée depuis le dernier appel, elle rend 1 chaque période, est elle est de la valeur 0 pendant la durée d'une période. C'est possible que le numéro de période arrive à la fin de la chronologie et il repasse au début, mais le vieil numéro de période est enregistré,dans ce cas la fonction va rendre la valeur 2.


**Quel est le numéro de l'abonné I2C de l'écran ?**

Un abonné I2C dispose d'une adresse sur le bus (numéro d'abonné). Dans le cas général un abonné contient de la mémoire adressable.


**Modifier le programme initial pour afficher "bonjour" sur l'Oled toutes les 2 secondes sans changer le comportement existant.**

On utilise les fonction de lib -   [​Repository API Ecran OLED](https://github.com/adafruit/Adafruit_SSD1306):
```c
// prepare to display
display.setTextSize(1); //font size
display.setTextColor(WHITE); // font color
display.println(mess); // string/number to display
display.setCursor(0,10); //set cursor to line 2 (font_size=1)
//display
display.display();
//clear display
display.ClearDisplay();
```
## Multi-Task
Ensuite on voudrais expérimenter la communications inter-tâches. On a besoin d'une boite à lettre et une variable d'état à 2 valeur indiquant l'état de la boite. La boite peut être vide ou pleine. Chaque tâche ne peut écrire que lorsque la boite est vide. Lorsqu'elle est vide, il y écrit et il change l'état. Inversement, le lecteur attend qu'elle soit pleine. Lorsqu'elle est pleine, il la lit et change l'état. On modifie le code basé comme ci-dessous:
### Mess_st & Led_st

```c
struct Led_st Led1;
struct Mess_st Mess1;
void setup() {
//period 1s = 1000000
setup_Led(&Led1, 0, 1500000, 13);// Led est exécutée toutes les 1.5s
setup_Mess(&Mess1, 1, 2000000, "OMG OH LALA");// Mess est exécutée toutes les 2 secondes
}

void loop() {
loop_Led(&Led1);
loop_Mess(&Mess1);
}
```



### Mailbox_st
```c
struct mailbox_st mb0;
void setup() {
setup_T1(&mb0,0);
setup_T2();
setup_T3(&mb0);
}
void loop() {
loop_T1_sender(&mb0);
loop_T2_reciever(&mb0);
loop_T3_LED(&mb0);
}
```
## Connection avec Raspberry

```c
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
//setup()s...
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
```
ensuit le Main fonction:

```c
struct mailbox_st mb0;
void setup() {
setup_mailbox(&mb0,0);
setup_oled();
//setup_led();
setup_RF24();
}
void loop() {
char buffer[32];
//loop_LED(&mb0);//-----problem here : LED blink frequency
loop_read(&mb0);

if( radio.available()){
radio.read( buffer, sizeof(buffer) ); // Get the payload
Serial.println(buffer);
sprintf(mb0.mess,"lalala");
}else{
loop_display(&mb0);
}
}
```
