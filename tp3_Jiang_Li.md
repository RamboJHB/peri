Compte Rendu PERI TP3
===================
Jiang 3602103 & Li 3770906


1.Configuration des GPIO pour le pilotage de l'écran 
--------------------------------------------
**Comment faut-il configurer les GPIOs pour les différents signaux de l'afficheur LCD ?**
Il faut configurer les GPIOs selon le mapping des GPIO pour les différents signaux:RS(7),E(8) D4(22),D5(23), D6(24), D7(25) et on met les portes à l'état *OUT*.

**Comment écrire des valeurs vers le LCD ?**
Parce que l'afficheur LCD est en mode 4 bits. Les valeurs écrits vers le LCD sont transmises en deux étapes: les 4 bits de poids fort et ensuite les 4 bits de poids faible, sur les portes D4 - D5.

**Quelles valeurs doivent être envoyées vers l'afficheur pour réaliser l'initialisation ?**
Pour l'initialisation, on doit envoie les valeurs suivants dans l'ordre:
E = 0  ->    RS = 0 ->   00110011 et 00110010 pour DDR adresse ->
LCD_FUNCTIONSET ->LCD_DISPLAYCONTROL ->LCD_ENTRYMODESET -> et un falling edge de Enable.
Et chaque fois on envoie un command, l'afficheur doit attendre au mions 2000us.
**Comment demander l'affichage d'un caractère ?**
On met RS à 1, (dans les code on appelle la fonction *lcd_message()*). Chaque caractère correspente à 8 bits. Et il y a un délai minimum  de 37us après l'envoyer.
**Comment envoyer des commandes telles que : l'effacement de l'écran, le déplacement du curseur, etc. ?**
Pour l'effacement de l'écran, on appelle la fonction *lcd_clear()* qui contient les commandes correspondantes *lcd_command(LCD_CLEARDISPLAY)* et *lcd_command(LCD_RETURNHOME)*, les commandes sont définit en haut par des macro HEX(). 
Et pour le déplacement du curseur, on appelle la fonction *lcd_set_cursor(l,c)* qui contient les commandes correspondantes **lcd_command(0x80+DDRAM[(l)%4]+(c)%0x14)**.( *DDRAM[4] = { 0, 0x40, 0x14, 0x54 }*)

2.Fonctionnement de l'écran et fonctions de base
----------------------------------------
**Question:
Compléter la fonction lcd_strobe (nommée nybble dans le cours) qui permet de générer le signal E.**
On écrit dans la fonction lcd_strobe():
```C
void lcd_strobe(void){
  gpio_update(E, 1);
  usleep(1);
  gpio_update(E, 0);
}
```
**Question:
Compléter la fonction lcd_write4bits en vous aidant du cours.**
On écrit dans la fonction lcd_write4bits():
```C
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
```
**Question:
Compléter la fonction lcd_init qui réalise la séquence d'initialisation telle que vue en cours. Vérifiez son fonctionnement sur une carte Raspberry Pi. Vous devez être en mode 4 bits, 2 lignes, matrix5x8 ; allumer l'écran sans curseur ; demander d'écrire de gauche à droite sans déplacement des caractères.**

On modifie les commandes dans la fonction lcd_init():
```C
void lcd_init(void){
  gpio_update(E, 0);
  lcd_command(HEX(0011,0011)); /* initialization */ //--------DDRAM address
  lcd_command(HEX(0011,0010)); /* initialization */ //--------DDRAM address
  lcd_command(LCD_FUNCTIONSET    | LCD_FS_4BITMODE  | LCD_FS_2LINE | LCD_FS_5x8DOTS  );
  lcd_command(LCD_DISPLAYCONTROL | LCD_DC_DISPLAYON | LCD_DC_CURSOROFF ); //sans cursor
  lcd_command(LCD_ENTRYMODESET   | LCD_EM_RIGHT     | LCD_EM_DISPLAYNOSHIFT); //sans déplacement
}
```
**Question:
1. Ecrire une fonction lcd_set_cursor qui positionne le curseur aux coordonnées (x,y) avec x la colonne, y la ligne.**
```C
static const DDRAM[4] = { 0, 0x40, 0x14, 0x54 };
#define CUR_SET(l,c) (0x80+DDRAM[(l)%4]+(c)%0x14)
void lcd_set_cursor(int x, int y){
	x--;
	y--;
	lcd_command(CUR_SET(x,y));
}
```

3.Affichage du monitoring système
------------------------------------------------------------------
**Question: Ecrivez un programme qui lit le contenu du fichier /proc/loadavg et qui affiche son contenu sur l'afficheur avec une fréquence d'actualisation de 1 seconde**
Les fonction principales : *read()* , *lcd_message()*
**Parser le contenu du fichier /proc/loadavg pour afficher des valeurs plus facilement compréhensibles.**
Les fonction principales :
*lseek(fd , 0, SEEK_SET);*
*read(fd , buf ,64);*
*strtok(buf , s);*
**----Veuillez regarder les codes pour les questions ci-dessus.**
4.Driver basique pour le LCD 
-------------------------------------------------------
On modifie la driver du TP2:
```C
//------------------------------------------------------------------------------
//                                Functions
//------------------------------------------------------------------------------

static int open_lcd(struct inode *inode, struct file *file) {	//open & select pin/gpio function
    printk(KERN_DEBUG "open()__________________JL\n");
    //Setting up GPIOs to outp**----Veuillez regarder le code pour les questions ci-dessus.**ut 
	gpio_config(RS, GPIO_OUTPUT);
	gpio_config(E , GPIO_OUTPUT);
	gpio_config(D4, GPIO_OUTPUT);
	gpio_config(D5, GPIO_OUTPUT);
	gpio_config(D6, GPIO_OUTPUT);
	gpio_config(D7, GPIO_OUTPUT);    	
	lcd_init();
	lcd_clear();
    return 0;
}
static ssize_t write_lcd(struct file *file,  char *buf, size_t count, loff_t *ppos) {
    lcd_clear();
    buf[14] = '\0';
    printk(KERN_DEBUG "write(%s)____JL\n",buf);
	lcd_message(buf);
    return count;
}
static int release_lcd(struct inode *inode, struct file *file) { //called in quitting program
    printk(KERN_DEBUG "close()_________________________JL\n");
	//lcd_clear();
    return 0;
}
struct file_operations lcd_fops ={
		.open       = open_lcd,
		.write      = write_lcd,
		.release    = release_lcd 
};// → vecteurs d'opérations
```


Puis les invoquer dans les fonctions du driver:

**----Veuillez regarder le code pour les questions ci-dessus.**
