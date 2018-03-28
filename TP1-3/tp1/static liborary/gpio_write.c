
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "mygpio.h"



void gpio_write ( int gpio_number , int val )
{
    if( val == 1 )
	GPIO_SET ( gpio_base, gpio_number );
    else
	GPIO_CLR ( gpio_base, gpio_number );

}

