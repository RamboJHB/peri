Compte Rendu PERI TP1
===================
Jiang 3602103 & Li 3770906

outils de développement
--------------------------
-ssh
-makefile 
-cross-compile
-static library ( c )

Contrôle de GPIO en sortie
--------------------------
-Dans exercice 1 on voudrait manipuler une pin du GPIO en mode "sortie" pour contrôler le clignotement d'une LED à une fréquence donnée, on modifie le code comme ci-dessous:
```c
while (1) {
    GPIO_SET ( gpio_base, GPIO_LED0 );    
    delay ( half_period );
    GPIO_CLR ( gpio_base, GPIO_LED0 );  
    delay ( half_period );
}
```

-Ensuite, on l'a compilé grâce au Makefile fourni et on a observé que les deux LEDs clignotent avec la fréquence définie.

-Pour changer la fréquence de clignotement, on peut simplement modifier la valeur de période.


Contrôle de plusieurs GPIO en mode "sortie"
-------------------------------------------

-En plus du GPIO 4 de l'exercice précédent, rajoutez la LED1 qui est connectée sur le GPIO 17:

```
#define GPIO_LED1   17
```
-Et puis on modifie le code dans main():
```c
while (1) {
        GPIO_SET ( gpio_base, GPIO_LED0 );    
        GPIO_CLR ( gpio_base, GPIO_LED1 );
        delay ( half_period );
        GPIO_CLR ( gpio_base, GPIO_LED0 ); 
        GPIO_SET ( gpio_base, GPIO_LED1 );  
        delay ( half_period );
  }
```

Switch de LED
--------------------------------------

-Maintenant on veut écrire un programme qui configure la GPIO 18 en entrée.

```    
GPIO_CONF_AS_INTPUT ( gpio_base, GPIO_BTN0 );
//...
while (1) {
	delay (20);
	val_nouv = GPIO_VALUE (gpio_base , GPIO_BTN0);
        if  ((val_prec != val_nouv) && (val_nouv == 0))//descending edge triggered
		BP_ON = 1;
	if ( (BP_ON == 1)){
		if (led != 1){
			GPIO_SET ( gpio_base, GPIO_LED0 );
        	GPIO_SET ( gpio_base, GPIO_LED1 );
			printf("LED lighted\n");
			led = 1;
		}else{
			GPIO_CLR ( gpio_base, GPIO_LED0 );
        		GPIO_CLR ( gpio_base, GPIO_LED1 );
			printf("LED x \n");
			led = 0;		
		} 
    }
    BP_ON = 0; 
	val_prec = val_nouv;
} 
```
Réalisation d'une petite librairie statique en c
----------------------------------
Écrire les fonctions suivantes et fabriquer une fonction libgpio.a
   gpio_init : ouverture et mapping. (gpio_config.c)
   gpio_setup : setup des broches *e.g. direction* (gpio_setup.c)
   gpio_read : lecture d'une broche (gpio_read.c)
   gpio_write : écriture d'une broche (gpio_write.c)
```
static uint32_t volatile * gpio_base = 0;
int gpio_init (){
     void * mmap_result;
     int result;
    mmap_fd = open ( "/dev/mem", O_RDWR   O_SYNC );
     if ( mmap_fd < 0 ) {
         printf ( "-- error: cannot setup mapped GPIO.\n" );
         return -1;
     }
     mmap_result = mmap (
         NULL
       , RPI_BLOCK_SIZE
       , PROT_READ   PROT_WRITE
       , MAP_SHARED
       , mmap_fd
       , BCM2835_GPIO_BASE );
     if ( mmap_result == MAP_FAILED ) {
         close ( mmap_fd );
         printf ( "-- error: cannot setup mapped GPIO.\n" );
         return -1;
    }
    return 0;
  }
```
```
//macros......
int  gpio_setup ( int gpio , int dir ){
    if (dir == 1) {
	    GPIO_CONF_AS_OUTPUT ( gpio_base, gpio );
	    return 0;
    }
    if (dir == 0){
	    GPIO_CONF_AS_INTPUT ( gpio_base, gpio );
	    return 0;
    }
    return -1
}
```
```
//macros......
int  gpio_read ( int gpio , int *buf){
    snprintf(buf,sizeof(buf),GPIO_VALUE( gpio_base, gpio));
    return 0;
}
```
```
//macros......
void gpio_write ( int gpio_number , int val )
{
    if( val == 1 )
	GPIO_SET ( gpio_base, gpio_number );
    else
	GPIO_CLR ( gpio_base, gpio_number );
}
```


Trouvez ci-joints les fichiers
