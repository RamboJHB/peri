/* ------------------ */
/* --- filetrNR.h --- */
/* ------------------ */

/*
 * Copyright (c) 2011-2015 Lionel Lacassagne, all rights reserved,  LRI,  University Paris Sud 11
 * Copyright (c) 2015-2016 Lionel Lacassagne, all rights reserved,  LIP6, UPMC
 */

#ifndef __FILTER_NR_H__
#define __FILTER_NR_H__

#ifdef __cplusplus
extern "C" {
#endif

// allocation / desallocation d'un noyau 1D et 2D
float32*  alloc_f32kernel1D(int radius);
float32** alloc_f32kernel2D(int radius);
    
void free_f32kernel1D(float32  *K, int radius);
void free_f32kernel2D(float32 **K, int radius);
    
// affichage
void display_f32kernel1D(float32  *K, int radius, char *format, char *name);
void display_f32kernel2D(float32 **K, int radius, char *format, char *name);
    
// initialisation
void init_gaussian_f32kernel1D(float32  *K, int radius, float32 sigma);
void init_gaussian_f32kernel2D(float32 **K, int radius, float32 sigma);
void init_average_f32kernel1D (float32  *K, int radius);
void init_average_f32kernel2D (float32 **K, int radius);

// convolution par un noyau 1D ou 2D
void convolve_f32kernel1D_ui8vector(uint8  *X,                 int j0, int j1, float32  *K, int radius, uint8  *Y);
void convolve_f32kernel2D_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, float32 **K, int radius, uint8 **Y);

// filtre median
void median_ui8vector(uint8  *X,                 int j0, int j1, int radius, uint8  *Y);
void median_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, int radius, uint8 **Y);
    
void median3_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1,         uint8 **Y);
void moyen_ui8matrix  (uint8 **X, int i0, int i1, int j0, int j1, int w,  uint8 **Y);

// nouvelles fonctions de filtrage
void fir_average_f32(uint8 *X, int n, int radius,        uint8 *Y);
void fir_average_i16(uint8 *X, int n, int radius,        uint8 *Y);
void fir_average_q16(uint8 *X, int n, int radius, int q, uint8 *Y);
  
void fir_gauss_f32(uint8 *X, int n, float32 sigma, uint8 *Y);

void iir_f32(uint8 *X, int n, float32 alpha,        uint8 *Y);
void iir_q16(uint8 *X, int n, float32 alpha, int q, uint8 *Y);
void iir_q32(uint8 *X, int n, float32 alpha, int q, uint8 *Y);

#ifdef __cplusplus
}
#endif

#endif // __FILTER_NR_H__