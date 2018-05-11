/* --------------- */
/* --- gauss.h --- */
/* --------------- */

/*
 * Copyright (c) 2011 Lionel Lacassagne, all rights reserved
 * University Paris Sud 11
 */

#ifndef __GAUSS_H__
#define __GAUSS_H__

#ifdef __cplusplus
extern "C" {
#endif
    
float32 gauss1D(float32 sigma, float32 x);
float32 gauss2D(float32 sigma, float32 x, float32 y);

#ifdef __cplusplus
}
#endif

#endif // __GAUSS_H__