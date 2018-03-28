#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>/*

#include <fcntl.h>*/
#include <sys/mman.h>
#include <unistd.h>



#include "mygpio.h"

int  gpio_read ( int gpio , int *buf){
    snprintf(buf,sizeof(buf),GPIO_VALUE( gpio_base, gpio));
    return 0;
}
