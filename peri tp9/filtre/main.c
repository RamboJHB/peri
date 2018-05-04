/* -------------- */
/* --- main.c --- */
/* -------------- */

/*
 * Copyright (c) 2011 Lionel Lacassagne, all rights reserved
 * University Paris Sud 11
 */

#include <stdio.h>
#include <math.h>
#include "test_filterNR.h"

// -------------------------------------
int main (int argc, const char * argv[])
// -------------------------------------
{
    test_filterNR(argc, argv);
    
    puts("bye ...");
    return 0;
}
