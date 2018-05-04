/* ---------------- */
/* --- random.h --- */
/* ---------------- */

// Random number generator
// Copyright (c) Numerical Recipes in C www.nr.com

#ifndef __RANDOM_H__
#define __RANDOM_H__

#ifdef __cplusplus
extern "C" {
#endif

float ran0(long *idum);
float ran1(long *idum);
float ran2(long *idum);
float ran3(long *idum);
float gasdev(long *idum);
    
#ifdef __cplusplus
}
#endif

#endif // __RANDOM_H__