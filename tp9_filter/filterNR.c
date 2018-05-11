/* ------------------ */
/* --- filterNR.c --- */
/* ------------------ */

/*
 * Copyright (c) 2011-2015 Lionel Lacassagne, all rights reserved,  LRI,  University Paris Sud 11
 * Copyright (c) 2015-2016 Lionel Lacassagne, all rights reserved,  LIP6, UPMC
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "def.h"
#include "nrutil.h"
//#include "util.h"
//#include "kernel.h"

//#include "all.h"
//#include "mem.h"
//#include "outils.h"

#include "gauss.h"
#include "filterNR.h"

// -----------------------------------
float32* alloc_f32kernel1D(int radius)
// -----------------------------------
{
    float32 *K;
    K = f32vector(-radius, radius);
    return K;
}
// ------------------------------------
float32** alloc_f32kernel2D(int radius)
// ------------------------------------
{
    float32 **K;
    K = f32matrix(-radius, radius, -radius, radius);
    return K;
}
// ------------------------------------------
void free_f32kernel1D(float32 *K, int radius)
// ------------------------------------------
{
    free_f32vector(K, -radius, radius);
}
// -------------------------------------------
void free_f32kernel2D(float32 **K, int radius)
// -------------------------------------------
{
    free_f32matrix(K, -radius, radius, -radius, radius);
}
// -----------------------------------------------------------------------
void display_f32kernel1D(float32 *K, int radius, char *format, char *name)
// -----------------------------------------------------------------------
{
    display_f32vector(K, -radius, +radius, format, name);
}
// ------------------------------------------------------------------------
void display_f32kernel2D(float32 **K, int radius, char *format, char *name)
// ------------------------------------------------------------------------
{
    display_f32matrix(K, -radius, +radius, -radius, +radius, format, name);
}
// ------------------------------------------------------------------
void init_gaussian_f32kernel1D(float32 *K, int radius, float32 sigma)
// ------------------------------------------------------------------
{
    int i;
    float32 x, sx;
    
    sx = 0.0f;
    for(i = -radius; i <= radius; i++) {
        x = gauss1D(sigma, i);
        K[i] = x;
        sx += x; // sommation pour normalisation
    }
    
    for(i=-radius; i<=radius; i++) {
        K[i] /= sx; // normalisation
    }
}
// -------------------------------------------------------------------
void init_gaussian_f32kernel2D(float32 **K, int radius, float32 sigma)
// -------------------------------------------------------------------
{
    int i, j;
    float32 x, sx;
    
    sx = 0.0f;
    for(i=-radius; i<=radius; i++) {
        for(j=-radius; j<=radius; j++) {
            x = gauss2D(sigma, i, j);
            K[i][j] = x;
            sx += x; // sommation pour normalisation
        }
    }
    for(i=-radius; i<=radius; i++) {
        for(j=-radius; j<=radius; j++) {
            K[i][j] /= sx;  // normalisation
        }
    }
}
// --------------------------------------------------
void init_average_f32kernel1D(float32 *K, int radius)
// --------------------------------------------------
{
    int i;
    float32 x, sx = (2*radius+1);
    
    for(i=-radius; i<=radius; i++) {
        x = 1.0f / sx;
        K[i] = x;
    }
}
// ---------------------------------------------------
void init_average_f32kernel2D(float32 **K, int radius)
// ---------------------------------------------------
{
    int i, j;
    float32 x, sx = (2*radius+1)*(2*radius+1); // la somme est connue
    
    for(i=-radius; i<=radius; i++) {
        for(j=-radius; j<=radius; j++) {
            x = 1.0f / sx;
            K[i][j] = x;
        }
    }
}
// --------------------------------------------------------------------------------------------
void convolve_f32kernel1D_ui8vector(uint8 *X, int j0, int j1, float32 *K, int radius, uint8 *Y)
// --------------------------------------------------------------------------------------------
{
    // fait l'hypothese qu'il y a des bords alloues
    int j;
    int jj;
    
    float32 y;
    
    for(j=j0; j<=j1; j++) {
        
        // convolution 1D
        y = 0.0f;
        for(jj=-radius; jj<=radius; jj++) {
            y += X[j+jj] * K[jj];
        }
        
        // clamping [0..255]
        if(y < 0.0f) y = 0.0f;
        if(y > 255.0f) y = 255.0f;
        
        Y[j] = (uint8) lroundf(y);
    }
}
// ---------------------------------------------------------------------------------------------------------------
void convolve_f32kernel2D_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, float32 **K, int radius, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------------
{
    int i, j;
    int ii, jj;
    
    uint8 **T;
    float32 y; 
    
    // tableau temporaire avec bords
    T = ui8matrix(i0-radius, i1+radius, j0-radius, j1+radius); 
    zero_ui8matrix(T, i0-radius, i1+radius, j0-radius, j1+radius);
    dup_ui8matrix(X, i0, i1, j0, j1, T);
    extend_ui8matrix(T, i0, i1, j0, j1, radius);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            
            // convolution 2D
            y = 0.0f;
            for(ii=-radius; ii<=radius; ii++) {
                for(jj=-radius; jj<=radius; jj++) {
                    y += T[i+ii][j+jj] * K[ii][jj];
                }
            }
            
            // clamipng
            if(y < 0.0f) y = 0.0f;
            if(y > 255.0f) y = 255.0f;

            Y[i][j] = (uint8) y;
        }
    }
    free_ui8matrix(T, i0-radius, i1+radius, j0-radius, j1+radius); 
}
/* ------------------------------------------------- */
void sort_selection_ui8vector(uint8 *X, int i0, int i1)
/* ------------------------------------------------- */
{
}
/* --------------------------------------------------------------------------------- */
void median_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, int radius, uint8 **Y)
/* --------------------------------------------------------------------------------- */
{    
}
// --------------------------------------------------------
void fir_average_f32(uint8 *X, int n, int radius, uint8 *Y)
// --------------------------------------------------------
{
    int i, di;
    int d; // diametre du filtre
    float32 s, y;
    uint8 *T; // tmp table with edge
    
    d = 2*radius+1;
    
    // allocation et copie dans un tableau temporaire avec bords
    T = ui8vector(0-radius, n-1+radius);
    dup_ui8vector(X, 0, n-1, T);//------------------- cp X to T
    extend_ui8vector(T, 0, n-1, radius); // T (-r, n-1+r )
    
    for(i = 0; i < n; i++) {
        
        s = 0.0f;
        for(di = -radius; di <= radius; di++) {
            s += T[i+di];
        }
        y = s / d;
        Y[i] = (uint8) lroundf(y); // float y round to the nearest integer value
    }
    
    free_ui8vector(T, 0-radius, n-1+radius); //free tmp table
}
// --------------------------------------------------------
void fir_average_i16(uint8 *X, int n, int radius, uint8 *Y)
// --------------------------------------------------------
{
    // normalisation par division
    int i, di;
    int d = 2*radius+1; // diametre du filtre
    uint16 s, y;
    uint8 *T; // tmp table with edge

    // allocation et copie dans un tableau temporaire avec bords
    T = ui8vector(0-radius, n-1+radius);
    dup_ui8vector(X, 0, n-1, T);
    extend_ui8vector(T, 0, n-1, radius); // duplication des bords
    
    // CODE A COMPLETER ICI
    for(i = 0; i < n; i++) {
        
        s = 0;
        for(di = -radius; di <= radius; di++) {
            s += T[i+di];
        }
        //y = s / d; 
        y = ( s + d / 2 ) / d;
        Y[i] = (uint8)y; 
    }
    free_ui8vector(T, 0-radius, n-1+radius); //free tmp table
}
// ---------------------------------------------------------------
void fir_average_q16(uint8 *X, int n, int radius, int q, uint8 *Y)//-----1/d = lambda / 2^q
// ---------------------------------------------------------------
{
    // normalisation par fraction equivalente
    int i, di;
    int d = 2*radius+1; // diametre du filtre
    uint16 s, y, lambda =( 1 << q )/ d; // d = 5 lambda = 51 ;
    uint8 *T; // tmp table with edge
    
    // allocation et copie dans un tableau temporaire avec bords
    T = ui8vector(0-radius, n-1+radius);
    dup_ui8vector(X, 0, n-1, T);
    extend_ui8vector(T, 0, n-1, radius); // duplication des bords
    
    // CODE A COMPLETER ICI
    for(i = 0; i < n; i++) {
        
        s = 0;
        for(di = -radius; di <= radius; di++) {
            s += T[i+di];
        }
        //y = s / d; 
		y = ( lambda *s + (1 << (q-1))) >> q;
        Y[i] = (uint8)y; 
    }
    free_ui8vector(T, 0-radius, n-1+radius);
}
// ---------------------------------------------------------
void fir_gauss_f32(uint8 *X, int n, float32 sigma, uint8 *Y)
// ---------------------------------------------------------
{
    int radius;
    uint8   *T; // tableau temporaire avec bords
    float32 *K; // kernel

    // allocation du kernel
    //printf("[gauss] sigma = %.1f\n", sigma);
    radius = (int) ceilf(2.0f * sigma) + 1; // relation entre rayon et sigma
    //printf("[gauss] radius = %d\n", radius);
    K = alloc_f32kernel1D(radius);
    init_gaussian_f32kernel1D(K, radius, sigma);
    //display_f32kernel1D(K, radius, "%6.3f", "Gauss kernel");
    
    // allocation et copie dans un tableau temporaire avec bords
    T = ui8vector(0-radius, n-1+radius);
    dup_ui8vector(X, 0, n-1, T);
    extend_ui8vector(T, 0, n-1, radius); // duplication des bords
    
    convolve_f32kernel1D_ui8vector(T, 0, n-1, K, radius, Y);
    
    free_ui8vector(T, 0-radius, n-1+radius);
}
// ---------------------------------------------------
void iir_f32(uint8 *X, int n, float32 alpha, uint8 *Y)
// ---------------------------------------------------
{
    int i;
    int radius = 2;
    
    float32 x0, y0, y1, y2;
    
    float32 gamma;
    float32  b0, a1, a2;
    
    uint8 *T; // tableau temporaire avec bords
    
    gamma = exp(-alpha);
    
    b0 = (1.0f - gamma) * (1.0f - gamma);
    a1 = 2.0f * gamma;
    a2 = - gamma * gamma;
    
    y1 = y2 = X[0];
    printf("iir_f32 : ");
    for(i = 0; i < n; i++) {
        
        x0 = X[i];
        y0 = b0 * x0 + a1 * y1 + a2 * y2;
        
        if(y0 <   0.0f) y0 =   0;
        if(y0 > 255.0f) y0 = 255;
        
        Y[i] = (uint8) lroundf(y0);
        y2 = y1; y1 = y0;
        printf("y[%d]=%f ",i,y0);
    }
    printf("\n");
}
// ----------------------------------------------------------
void iir_q16(uint8 *X, int n, float32 alpha, int q, uint8 *Y)
// ----------------------------------------------------------
{
    // CODE A COMPLETER ICI
    int i;
    int radius = 2;
    sint16 Q = 1 << q;//   Q = 2^q
    sint16 QQ = Q >> 1;//   Q = 2^(q-1)

    sint16 x0, y0, y1, y2;

    float32 gamma = exp(-alpha);
    float32 b0= (1.0f - gamma) * (1.0f - gamma);
    float32 a1 = 2.0f * gamma; 
    float32 a2 = - gamma * gamma;
    
    //sint32 B0 = Q*(sint32)b0;
    //sint32 A1 = Q*(sint32)a1;
    //sint32 A2 = Q*(sint32)a2;
    sint16 B0 = (sint16)b0 << q;
    sint16 A1 = (sint16)a1 << q;
    sint16 A2 = (sint16)a2 << q;
    
    uint8 *T; // tableau temporaire avec bords

    y1 = y2 = (sint16) X[0];
    printf("iir_q16 : ");
    for(i = 0; i < n; i++) {
        
        x0 = (sint16) X[i];
	    // y0 = (2^q * b0 * x0 + 2^q * a1 * y1 + 2^q * a2 * y2 )/ 2^q
                
        y0 = (B0 * x0 + A1 * y1 + A2 * y2 + QQ )>>q;
        if(y0 <   0) y0 =   0;
        if(y0 > 255) y0 = 255;
        
        Y[i] = (uint8)y0;
        y2 = y1; y1 = y0;
	    printf("y[%d]=%hd ",i,y0);
    }   
    printf("\n");
}
// ----------------------------------------------------------
void iir_q32(uint8 *X, int n, float32 alpha, int q, uint8 *Y)
// ----------------------------------------------------------
{
    // CODE A COMPLETER ICI
    int i;
    int radius = 2;
    sint32 Q = 1 << q;//   Q = 2^q
    sint32 QQ = Q << q;
    sint32 QQQ = Q >> 1 ;

    sint32 x0, y0, y1, y2;

    float32 gamma = exp(-alpha);
    float32 b0= (1.0f - gamma) * (1.0f - gamma);
    float32 a1 = 2.0f * gamma; 
    float32 a2 = - gamma * gamma;
    

    sint32 B0 = (sint32)b0 << q;
    sint32 A1 = (sint32)a1 << q;
    sint32 A2 = (sint32)a2 << q;
    
    uint8 *T; // tableau temporaire avec bords

    y1 = y2 = (sint32) X[0];
    
    for(i = 0; i < n; i++) {
        
        x0 = (sint32) X[i];
        sint32 X0 = (sint32)x0 << q;
        sint32 Y1 = (sint32)y1 << q;
        sint32 Y2 = (sint32)y2 << q;
	    // y0 = (2^q * b0 * x0 + 2^q * a1 * y1 + 2^q * a2 * y2 )/ 2^q
                
        y0 = (B0 * X0 + A1 * Y1 + A2 * Y2 + QQQ )/QQ;
        if(y0 <   0) y0 =   0;
        if(y0 > 255) y0 = 255;

        printf("y[%d]=%d ",i,y0);
        Y[i] = (uint8)y0;
        y2 = y1; y1 = y0;
	    
    } 
    printf("\n");
}
