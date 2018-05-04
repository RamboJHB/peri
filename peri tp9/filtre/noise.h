/* --------------- */
/* --- noise.h --- */
/* --------------- */

/*
 * Copyright (c) 2011 Lionel Lacassagne, all rights reserved
 * University Paris Sud 11
 */

#ifndef __NOISE_H__
#define __NOISE_H__

#ifdef __cplusplus
extern "C" {
#endif
  
// based on NRC files

float32 gaussian_noise(float32 average, float32 sigma);

void gaussian_noise_ui8vector(uint8 *X, int i0, int i1, float32 sigma,   uint8 *Y);
void impulse_noise_ui8vector (uint8 *X, int i0, int i1, float32 percent, uint8 *Y);

void gaussian_noise_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, float32 sigma,   uint8 **Y);
void impulse_noise_ui8matrix (uint8 **X, int i0, int i1, int j0, int j1, float32 percent, uint8 **Y);

float32 mean_square_error(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
    
float32 psnr_ui8vector(uint8  *X, int i0, int i1, uint8 *Y);
float32 psnr_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
    
#ifdef __cplusplus
}
#endif

#endif // __NOISE_H__