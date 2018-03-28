/*
 * This file is a template for the first Lab in the SESI-PERI module.
 *
 * It draws inspiration from Dom & Gert code on ELinux.org.
 *
 * Authors:
 *   Julien Peeters <julien.peeters@lip6.fr>
 */

/*
 * Headers that are required for printf and mmap.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/*
 * Main program.
 */

#include "mygpio.h"

#define GPIO_LED0   4
#define GPIO_LED1   17
#define GPIO_BTN0   18


void delay ( unsigned int milisec )
{
    struct timespec ts, dummy;
    ts.tv_sec  = ( time_t ) milisec / 1000;
    ts.tv_nsec = ( long ) ( milisec % 1000 ) * 1000000;
    nanosleep ( &ts, &dummy );
};

int main ( int argc, char **argv )
{
    int                 result;
    int                 period, half_period;

    /* Retreive the mapped GPIO memory. */
    result = gpio_init ( &gpio_base );

    if ( result < 0 ) {
        printf ( "-- error: cannot setup mapped GPIO.\n" );
        exit ( 1 );
    }

    period = 1000; /* default = 1Hz */

    if ( argc > 1 ) {
        period = atoi ( argv[1] );
    }
    half_period = period / 2;

    /* Setup GPIO of LED0 to output. */
    GPIO_CONF_AS_OUTPUT ( gpio_base, GPIO_LED0 );
    /* Setup GPIO of LED1 to output. */
    GPIO_CONF_AS_OUTPUT ( gpio_base, GPIO_LED1 );


    printf ( "-- info: start blinking.\n" );


    while (1) {
	    delay (20);
	if ( GPIO_VALUE (gpio_base , GPIO_BTN0) ){
			GPIO_SET ( gpio_base, GPIO_LED0 );
        		delay ( half_period );
        		GPIO_CLR ( gpio_base, GPIO_LED0 );
        		delay ( half_period );
        		GPIO_SET ( gpio_base, GPIO_LED1 );
        	} else {
        		GPIO_CLR ( gpio_base, GPIO_LED0 );
        		GPIO_CLR ( gpio_base, GPIO_LED1 );
	}
    }    
    return 0;
}
