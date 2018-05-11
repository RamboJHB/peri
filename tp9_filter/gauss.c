/* --------------- */
/* --- gauss.c --- */
/* --------------- */

/*
 * Copyright (c) 2011 Lionel Lacassagne, all rights reserved
 * University Paris Sud 11
 */

#include <math.h>

#include "def.h"
#include "gauss.h"

/* ----------------------------------- */
float32 gauss1D(float32 sigma, float32 x)
/* ----------------------------------- */
{
    float32 pi = 3.1415927;
    float32 r;
    r = 1.0f / (sqrt(2*pi)*sigma) * exp(-(x*x)/(2*sigma*sigma));
    return r;
}
/* ---------------------------------------------- */
float32 gauss2D(float32 sigma, float32 x, float32 y)
/* ---------------------------------------------- */
{
    float32 pi = 3.1415927;
    float32 r;

    r = 1.0f / (2*pi*sigma*sigma) * exp(-(x*x+y*y)/(2*sigma*sigma));
    return r;
}
