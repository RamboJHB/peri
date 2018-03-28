Compte Rendu PERI TP2
===================
Jiang 3602103 & Li 3770906


Étape 1 : création et test d'un module noyau 
--------------------------------------------
Après avoir créé le module, on a modifié la valeur des variables CARD_NUMB, LOGINet LAB dans makefile afin de les adapter respectivement au numéro de la carte choisie, comme ci-dessous:
```
  CARD_NUMB       = 22
  ROUTER          = 132.227.102.36
  LOGIN           = Jiang_Lee
  LAB             = lab2

exe : test.x
	scp -P50$(CARD_NUMB) test.x pi@$(ROUTER):$(LOGIN)/$(LAB)
test.x: test.c
	bcm2708hardfp-gcc -o $@ $< -O2 -static
```

Et puis on compile le module avec la commande *make*, ensuite on copie module.ko dans le répertoire ciblé sur la RaspberryPi avec *scp* avec la commande *make upload*. 
Et *make exe* pour compilation de *test.c* qui utilise le driver et le envoyer à la carte dans étape 3.

Dans le répertoire Jiang_Lee/lab2 ,on charge le module: *sudo insmod ./module.ko* . Et puis on peut observer si le module est bien ajouté avec la commande *lsmod*.Et la commande *dmesg* permet d'afficher les messages du système informe 
du travail réalisé. Après on décharge le module avec *rmmod* et le vérifie avec *lsmod* et *dmesg*.

Étape 2 : ajout des paramètres au module
----------------------------------------
Dans cette étape on ajoute dans module.c deux paramètres: LED et BP, et indiquer le numéro de ports.
```C
static int LED; //=17 define at init dev
static int major;
static const int LED0 = 4;
static const int BTN0 = 18;    
            
module_param(LED, int, 0);
MODULE_PARM_DESC(LED, "Nombre de led");

 static int __init mon_module_init(void) {
     printk(KERN_DEBUG "Hello World !\n");
     printk(KERN_DEBUG "LED=%d !\n", LED);
     printk(KERN_DEBUG "BP=%d !\n", BP);
     return 0;
 }
```
Avec la commande *sudo insmod ./module.ko LED=17*  on définie le paramètre de LED. Et puis on voit que les paramètres ont été lus par *dmesg*.

Étape 3 : création d'un driver qui ne fait rien mais dans le noyau 
------------------------------------------------------------------

Maintenant on crée un module nommé ledbpJL en ajoutant un nouveau répertoire et en modifiant le Makefile. Pour enregistrer le driver dans ce module, on ajoute une fonction d'enregistrement dans la fonction init du module(on déclare une nouvelle variable statique qui s'appelle *major* au début du fichier): 

```C
 major = register_chrdev(0, "ledbpJL", &fops_ledbp); 
```

on considère aussi le déchargement dans la fonction exit du module:

```C
 unregister_chrdev(major, "ledbpJL");
```

Ensuite on a réussi à le compiler et le charger. Pour savoir le numéro *major*, on cherche dans le fichier /proc/devices et on trouve que le numéro major est 246. Et puis avec la commande  

```
 sudo mknod /dev/ledbpJL c 246 0
 sudo chmod a+rw /dev/ledbpJL
```

*mknod* c'est pour créer le noeud ledbpJL et *chmod* pour modifier les autorisations d'accès à ce ficher. Et puis on peut trouver le fichier ledbpJL dans le répertoire /dev, qui signifie que le device a été créé. On peut aussi tester le driver avec les commandes suivantes:

```
 echo "rien" > /dev/ledbpJL
 dd bs=1 count=1 < /dev/ledbpJL
 dmesg
```
Pour simplifier on crée deux scripts shells nommé *insdev* et *rmdev* pour automatiser le chargement.



Étape 4 : accès aux GPIO depuis les fonctions du pilote 
-------------------------------------------------------
Après la création du driver, on veux maintenant accéder aux GPIO depuis les fonctions du driver. L'adresse physique de base des GPIO est mappée dans l'espace virtuel du noyau.
On ajoute dans le ficher ledbpJL.c les fonctions gpio_fsel(), gpio_write() et gpio_read().

```C
static void gpio_fsel(int pin, int fun){
    uint32_t reg = pin / 10;
    uint32_t bit = (pin % 10) * 3;
    uint32_t mask = 0b111 << bit;
    gpio_regs->gpfsel[reg] = (gpio_regs->gpfsel[reg] & ~mask) | ((fun << bit) & mask);
}

static uint32_t gpio_read(int pin){
     return ((gpio_regs->gplev[pin / 32]) >> (pin % 32)) & 0x1;  
}

static void gpio_write(int pin, bool val){
    if (val)
        gpio_regs->gpset[pin / 32] = (1 << (pin % 32));
    else
        gpio_regs->gpclr[pin / 32] = (1 << (pin % 32));
}
```
Puis les invoquer dans les fonctions du driver:

```C
static int open_ledbp(struct inode *inode, struct file *file) {	//open & select pin/gpio function
    printk(KERN_DEBUG "open()__________________JL\n");
    gpio_fsel(BTN0,0);	
    gpio_fsel(LED0,1);
    gpio_fsel(LED,1);
    return 0;
}

static ssize_t read_ledbp(struct file *file, char *buf, size_t count, loff_t *ppos) {//read button
    if (gpio_read(BTN0) == 1) 
	buf[0] = '1';
    else
        buf[0] = '0';
    printk(KERN_DEBUG "read(%c)____JL\n",buf[0]);
    return count;
}

static ssize_t write_ledbp(struct file *file, const char *buf, size_t count, loff_t *ppos) {//write LED
    printk(KERN_DEBUG "write(%c)____JL\n",buf[0]);
    int led[2];  
    led[0] = LED0;
    led[1] = LED;  
    int i;
    for ( i = 0 ; i <count ; i ++ ){
        if (buf[i] == '0')
            gpio_write(led[i],false);       
        else
            gpio_write(led[i],true);
    }

    return count;
}

static int release_ledbp(struct inode *inode, struct file *file) { //called in quitting program
    printk(KERN_DEBUG "close()_________________________JL\n");
    gpio_fsel(LED0, 0);    
    return 0;
}

```

Donc on peut accéder aux GPIO et contrôler le clignotement des LEDs en utilisant *test.c*.


Étape 5 : Usage d'un timer dans le noyau pour faire clignoter (optionnel)
-------------------------------------------------------------------------
Dans cette étape, à part de contrôler l'allumage, on veut aussi demander le clignotement en driver, on ajoute un TIMER pour le réaliser.  
```C
       static struct timer_list led_blink_timer;
       static int led_blink_period = 1000;
       ds
 static void led_blink_handler(unsigned long unused)
 {
     static bool on = false;
     on = !on;
     gpio_write(LED0, on);
     mod_timer(&led_blink_timer, jiffies + -msecs_to_jiffies(led_blink_period));
 }
```

Et puis on setup et delete le TIMER respectivement dans *init* :

```
    setup_timer(&led_blink_timer, led_blink_handler, 0);
    result = mod_timer(&led_blink_timer, jiffies + msecs_to_jiffies(led_blink_period));
    BUG_ON(result < 0);
```
 et *exit*:
```C
 del_timer(&led_blink_timer);
```
Trouvez ci-joints les fichiers
