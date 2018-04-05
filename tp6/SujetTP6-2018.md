{{{#!protected
= Programmation Arduino & communication NRF24L01

== Objectifs de la séance

Cette séance de TME est assez chargée puisqu'elle va avoir 4 objectifs
1. Découverte de l'arduino et premier programme
2. Programmation multitâches par composants
3. Utilisation de l'écosystème Arduino pour la programmation des périphériques
4. Communication point-à-point NRF24L01

== Remarques ==

Vous devez utiliser la version d'arduino qui se trouve :
{{{#!bash
> /opt/arduino-1.6.8/arduino
}}}

Les documents nécessaires se trouvent :
- [https://github.com/adafruit/Adafruit_SSD1306 Repository API Ecran OLED]
- [https://github.com/adafruit/Adafruit-GFX-Library Repository API Graphique]
- [http://www.mon-club-elec.fr/pmwiki_reference_arduino/pmwiki.php?n=Main.ReferenceMaxi Langage Arduino]

== Objectif ==

Le but de cette séance est de faire de la programmation sur Arduino en utilisant 3 entrées-sorties :
- La LED présente sur le module.
- L'écran OLED
- Le port série qui relie le module et le PC.

Nous voulons aussi expérimenter la programmation "multi-tâches". Les guillemets expriment le fait qu'il s'agit plus d'une structure du programme que d'une vraie programmation multi-tâches dans la mesure où il n'y a pas de système d'exploitation. Toutefois, les "tâches" seront programmées de sorte à être indépendantes de leur contexte d'utilisation et elles seront "ordonnancées" par la fonction loop().

== Exécution ''multi-tâches'' ==

Nous allons donc voir comment il est possible de programmer des applications multi-tâches coopératives dans l'environnement Arduino sans pour autant disposee des services d'un OS. Le code a été volontairement simplifié à l'extrême afin de bien comprendre le principe. ICI, toute l'application sera dans un seul fichier et nous n'allons pas utiliser la programmation objet pour ne pas complexifier (mais nous le ferons plus tard).

Chaque tâche est représentée par une fonction qui code son comportement.
Dans l'environnement Arduino, la fonction loop() s'exécute en boucle, c'est elle qui va séquencer l'exécution des tâches.
La fonction loop() demande donc l'exécution des tâches à tour de rôle.
Les tâches n'ont pas le droit de conserver le processeur sinon cela crée un blocage du système.
La structure générale d'une tâche est la suivante :

{{{#!c
void Tache(arguments) {
   // test de la condition d'exécution
   if (evement_attendu_absent) return;
   // code de la tache
   ....
}
}}}

Pour les tâches périodiques, nous pouvons écrire une fonction qui exploite un timer interne du processeur qui s'incrémente chaque microseconde. Cette fonction nommée `waitFor(int timer, unsigned long period)` prend deux paramètres `timer` et `period`. Le premier un numéro de timer (il en faudra autant que de tâches périodiques). Le second est une période en microsecondes. 

`wairFor()` peut être appelée aussi souvent que nécessaire, elle rend 1 une seule fois par période (second paramètre).
Si elle n'est pas appelée pendant longtemps alors elle rend le nombre de périodes qui se sont écoulées.

Dans l'application suivante nous avons deux tâches périodiques `Led()` et `Mess()`.
La première fait clignoter une led dont le numéro est passé en paramètre à 5Hz.
La seconde affiche bonjour à une fois par seconde.
{{{#!c
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
#define MAX_WAIT_FOR_TIMER 2
unsigned int waitFor(int timer, unsigned long period){
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];
  unsigned long newTime = micros() / period;              // numéro de la période modulo 2^32 
  int delta = newTime - waitForTimer[timer];              // delta entre la période courante et celle enregistrée
  if ( delta < 0 ) delta += 1 + (0xFFFFFFFF / period);    // en cas de dépassement du nombre de périodes possibles sur 2^32 
  if ( delta ) waitForTimer[timer] = newTime;             // enregistrement du nouveau numéro de période
  return delta;
}

void Led(int timer, long period, byte led) {
  static byte val = 0;                                    // valeur à mettre sur la led
  if (!waitFor(timer,period)) return;                     // sort s'il y a moins d'une période écoulée
  digitalWrite(led,val);                                  // ecriture
  val = 1 - val;                                          // changement d'état
}

void Mess(int timer, long period, const char * mess) {
  if (!(waitFor(timer,period))) return;                   // sort s'il y a moins d'une période écoulée
  Serial.println(mess);                                   // affichage du message
}

void setup() {
  pinMode(13,OUTPUT);                                     // initialisation de la direction de la broche
  Serial.begin(115200);                                   // initialisation du débit de la liaison série
}

void loop() {
  Led (0,100000,13);                                      // Led est exécutée toutes les 100ms
  Mess(1,1000000,"bonjour");                              // Mess est exécutée toutes les secondes
}
}}}

== Utilisation de l'écran ==

Nous allons utiliser un écran OLED connecté en I2C, 128x32 **ssd1306**
- La bibliothèque de l'écran se trouve en tapant la requête `ssd1306 arduino`[[BR]] à l'adresse   
  [https://github.com/adafruit/Adafruit_SSD1306].
  Vous devrez prendre également la bibliothèque GFX à l'adresse [https://github.com/adafruit/Adafruit-GFX-Library]
  qui est la bibliothèque graphique.   
- Vous pouvez exécuter l'exemple proposé dans la bibliothèque. Cette bibliothèque fonctionne pour plusieurs 
  types modèles. Vous allez choisir le bon exemple : 128x32 I2C.

**Questions**
- Que contient le tableau `waitForTimer[]`` ?
- Dans quel cas la fonction `waitFor() peut rendre 2 ?
- Quel est le numéro de l'abonné I2C de l'écran ?
- Modifier le programme initial pour afficher "bonjour" sur l'Oled toutes les 2 secondes sans changer le comportement existant.

== Communications inter-tâches ==

Lorsqu'on écrit un programme multi-tâches, il est intéressant de les faire communiquer. Pour ce faire, nous allons simplement créer variables globales et les donner en arguments aux taches communicantes. 

Supposons que nous voulions que la tâche T1 envoie un message à la tâche T2. Nous allons utiliser une boite à lettre. Le code suivant explique le principe qui est basé sur une variable d'état à 2 valeur indiquant l'état de la boite. La boite peut être vide ou pleine.
l'écrivain T1 ne peut écrire que lorsque la boite est vide. Lorsqu'elle est vide, il y écrit et il change l'état. Inversement, le lecteur attend qu'elle soit pleine. Lorsqu'elle est pleine, il la lit et change l'état.

Il s'agit d'une communication sans perte. Si T1 ne testait pas l'état de la boite, on pourrait avoir des pertes, c'est parfois nécessaire, si T2 n'a pas eu le temps d'utiliser la boite mais que T1 a une nouvelle valeur, il peut écraser la valeur présente.
{{{#!c
struct mailbox {
  enum {EMPTY, FULL} state;
  int val;
} mb0 = {.state = EMPTY};

void T1(&mb) {
  if (mb->state != EMPTY) return; // attend que la mailbox soit vide
  mb->val = 42;
  mb->state = FULL;
}

void T2(&mb) {
  if (mb->state != FULL) return; // attend que la mailbox soit pleine
  // usage de mb->val
  mb->state = EMPTY;
}
}}}

**Questions**

- Vous allez reprendre le programme ci-dessus qui fait clignoter la LED et afficher un message périodique sur l'écran Oled et le transformer de sorte à modifier son comportement par des commandes envoyé par l'utilisateur depuis le clavier du PC.

- Le programme est en attente d'un commande pour contrôler l'état de la LED et le message affiché sur l'écran Oled. Les commandes pourraient être :
  - A : met la lED en clignotement.
  - E : éteint la LED.
  - P message : change le message affiché sur l'écran.

  Le langage est volontairement simple pour que l'interprétation de la commande soit simple. 
  Commencez par les commandes de la LED 

  Quand cela marche ajouter la commande de message.

- Pour la tâche qui lit le clavier vous avez deux possibilités. Un tâche "normale" qui est ordonnancée par la fonction loop() ou une tâche dont l'exécution est déclenchée par l'arrivée d'un caractère. En effet, l'arrivée d'un caractère déclenche une interruption, il est possible de programmer une routine d'interruption ISR (Interrupt Service Routine). Faites une tâche "normale" et tenter de faire une tâche ISR.










== Récupération de la bibliothèque du NRF24L01 ==

Si nous voulons continuer à cross compiler, il faut installer la librairie qui va permettre de contrôler le module NRF24L01. Il existe plusieurs librairies. Celle choisie à le mérite d'être disponible dans l'environnement !RaspberryPi et !Arduino. C'est-à-dire que lorsque vous aurez compris comment l'utiliser avec la !RaspberryPi, le passage sur Arduino sera facile.

 * Aller sur le site https://github.com/tmrh20/RF24
 * Récupérer le .zip de la branche master (bouton `clone and download -> Download ZIP`) 
{{{#!bash
$ unzip RF24-master.zip
$ cd RF24-master
$ mkdir $HOME/rf24
$ ./configure --prefix=$HOME/rf24 --soc=BCM2835 --c_compiler=bcm2708hardfp-gcc --cxx_compiler=bcm2708hardfp-g++ --driver=SPIDEV --ldconfig=''
$ make
$ make install
}}}
 * Vérification que la library est installée.
{{{#!bash
$ ls $HOME/rf24
  include  lib
}}}

Cette même bibliothèque a été installée sur les cartes !RaspberryPi car la bibliothèque est dynamique et non pas statique, donc il faut la bibliothèque sur la !RaspberryPi.

== Documents de référence du module NRF24L01 ==

 * [http://www.nordicsemi.com/eng/Products/2.4GHz-RF/nRF24L01P Site Nordic nRF24L01Plus] 
 * [https://www.sparkfun.com/datasheets/Components/SMD/nRF24L01Pluss_Preliminary_Product_Specification_v1_0.pdf Spéicification nRF24L01plus]
 * [https://github.com/TMRh20/RF24 Repository API TMRh20/RF24] 

== Communication entre le capteur et la base ==

La documentation de la bibliothèque est [http://tmrh20.github.io/RF24/classRF24.html ici] dont voici un résumé :

* ` 	RF24 (uint8_t _cepin, uint8_t _cspin)`[[BR]]
  Configuration du module radio et du SPI, reçoit les numéros de broche cepin (radio) cspin (SPI Slave Select)
* `bool 	begin (void)`  Démarrage du module radio
* `void 	startListening (void)`
* `void 	stopListening (void)`
* `bool 	available (void)`]
* `void 	read (void *buf, uint8_t len)`
* `bool 	write (const void *buf, uint8_t len)`
* `void 	openWritingPipe (const uint8_t *address)`
* `void 	openReadingPipe (uint8_t number, const uint8_t *address)`

- **sensor** (sur l'Arduino)
{{{#!c
#include <SPI.h>
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10); // radio(CE,CS)

byte addresses[][6] = {"0Node"};

void setup() {
  Serial.begin(115200);
  printf_begin();
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.printDetails();
  delay(1000);
}


void loop() {
  Serial.println(F("Now sending !"));

  unsigned long start_time = millis();                             // Take the time, and send it.  This will block until complete
  if (!radio.write( &start_time, sizeof(unsigned long) )){
     Serial.println(F("failed!"));
  }
  
  delay(1000);
}
}}}


- **baseSensor** (sur la raspberry pi)
{{{#!c
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

typedef uint8_t byte;

using namespace std;

RF24 radio(15,0);

byte addresses[][6] = {"0Node","1Node","2Node","3Node","4Node","5Node"};

void setup() {
  radio.begin();
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1,addresses[0]);
  radio.printDetails();
  radio.startListening();
}

void loop() {
  unsigned long got_time;

  if( radio.available()){
     radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
     cout << got_time << endl;
   }
}

int main(int argc, char** argv){
    setup();
    while (1) loop();
    return 0;
}
}}}

- Makefile sur la raspberry pi
{{{#!make
RPI?=20
SRC=src
APP=NRF24L01_base
DST=lacas/nrf

CROSSDIR        = /users/enseig/franck/peri
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-

INC=$(HOME)/rf24/include
LIB=$(HOME)/rf24/lib
CFLAGS=-Wall -Wfatal-errors -O2 -I$(INC)
LDFLAGS=-L$(LIB) -lrf24

all:  $(APP).x

$(APP).x: $(APP).cpp
    $(CROSS_COMPILE)g++ -o $@ -I$(INC) $<  -O2 $(LDFLAGS)

upload: 
    scp -P50$(RPI) $(APP).x pi@peri:$(DST)

clean:
    rm -f *.o *.x *~
}}}

== Travail demandé ==

 * Le but initial est de lire la valeur envoyée par l'Arduino et de l'afficher sur le terminal de la raspberry. Il y a donc au moins 2 noeuds, un émetteur (l'arduino) et un récepteur (la raspberry).
 * Ensuite, l'idée est de faire une communication entre deux raspberry pi. Il va falloir que vous lisiez la documentation du NRF pour comprendre comment numéroter les noeuds. Une raspberry allume la led de sa voisine.
 * Comme, il n'y a pas assez de raspberry, nous allons ajouter des arduinos.
 * [http://www.mon-club-elec.fr/pmwiki_reference_arduino/pmwiki.php?n=Main.ReferenceMaxi Langage Arduino]

== Programmation des Arduinos ==

Vous devez utiliser la dernière version d'arduino qui se trouve `/opt/arduino-1.6.8/arduino`.
Vous commencerez par compiler le programme `blink` qui fait clignoter la led 13 présente sur l'arduino.
Pour pouvoir compiler un programme sur le NRF24, il faut ajouter la bibliothèque.

**L'usage des bibliothèques Arduino**

* Ce qu'il y a de bien dans l'écosystème Arduino, c'est la volonté de faire simple.
  En effet, pour presque tous les "périphériques" existants il existe un et même
  souvent plusieurs bibliothèques de fonctions écrites par des "amateurs" souvent très doués. 
  En plus, les sources sont ouvertes, et il est donc possible d'adapter ces codes pour des 
  besoins spécifiques.

* Les bibliothèques sont trouvées, en général, en tapant sur un moteur de recherche, la requête 
  "nom-du-module Arduino". Les projets sont souvent sur github. Pour faire court,
   * Vous téléchargez la bibliothèque (un RF24-master.zip)
   * Vous ajoutez la bibliothèque dans l'environnement Arduino (import Library)
   * Vous lancer l'IDE Arduino et dans le menu '''file/exemples''' vous avez un exemple (souvent plusieurs) de la nouvelle bibliothèque.
   * Vous en choisissez un, vous le chargez, vous le compilez, vous l'uploadez, vous le testez :-)

L'idée sera d'allumer la led de l'arduino depuis la raspberry pi ou l'inverse.

}}}