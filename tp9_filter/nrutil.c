/* -------------- */
/* --- nrutil --- */
/* -------------- */

/*
 * Copyright (c) 2011 Lionel Lacassagne, all rights reserved, University Paris Sud 11, University Pierre and Marie Curie
 * Copyright NRC (www.nrc.com) Cambridge
 */

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h> // va_list
#include <string.h> // strcmp
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h> /* fabs */

#include "def.h"
#include "nrutil.h"

//NR_END est maintenant defini dans nrutil.h

//#define NR_END 1
//#define FREE_ARG char*

long nr_end = NR_END;

/* ------------------------- */
void nrerror(char *format, ...)
/* ------------------------- */
/* global error handler */
{
    va_list pa;
    int n;
    char *s, c;
    float f;
    
    va_start(pa, format);
    
    while (*format != '\0') {
        if ( *format == '%' ) {
            switch (*++format) {
                case '%' : putchar('%'); break;
                case 'c' :
                    c = va_arg(pa, int); // 'char' is promoted to 'int' when passed through '...'
                    putchar(c);
                    break;
                case 'd' :
                    n = va_arg(pa, int);
                    printf("%d", n); 
                    break;
                case 'f' : /* affichage d'un float */ 
                    f = va_arg(pa, double);    // 'float' is promoted to 'double' when passed through '...'
                    printf("%f", f); 
                    break;
                case 's' :
                    s = va_arg(pa, char*);
                    for ( ; *s != '\0'; s++ ) 
                        putchar(*s);
                        break;
            }
        }
    else 
        putchar( *format);
    format++;
    }   
  va_end(pa);
    putchar('\n');
    exit(-1);
}
/* ------------------------------------------------------------------------------------- */
void generate_filename_k_ndigit(char *filename, int k, int ndigit, char *complete_filename)
/* ------------------------------------------------------------------------------------- */
{
  char *format = "%s%d.txt";

  switch(ndigit) {
    case 0 : format = "%s%d.txt";   break;
    case 1 : format = "%s%01d.txt"; break;
    case 2 : format = "%s%02d.txt"; break;
    case 3 : format = "%s%03d.txt"; break;
    case 4 : format = "%s%04d.txt"; break;
    case 5 : format = "%s%05d.txt"; break;
    case 6 : format = "%s%06d.txt"; break;
    case 7 : format = "%s%07d.txt"; break;
    case 8 : format = "%s%08d.txt"; break;
    case 9 : format = "%s%09d.txt"; break;
    default: format = "%s%02d.txt"; break;
  }
  sprintf(complete_filename, format, filename, k);
}
/* ---------------------------------------------------------------------------------------------------------------- */
void generate_filename_k_ndigit_extension(char *filename, int k, int ndigit, char *extension, char *complete_filename)
/* ---------------------------------------------------------------------------------------------------------------- */
{
  char *format = "%s%d.%s";

  switch(ndigit) {
    case 0 : format = "%s%d.%s";   break;
    case 1 : format = "%s%01d.%s"; break;
    case 2 : format = "%s%02d.%s"; break;
    case 3 : format = "%s%03d.%s"; break;
    case 4 : format = "%s%04d.%s"; break;
    case 5 : format = "%s%05d.%s"; break;
    case 6 : format = "%s%06d.%s"; break;
    case 7 : format = "%s%07d.%s"; break;
    case 8 : format = "%s%08d.%s"; break;
    case 9 : format = "%s%09d.%s"; break;
    default: format = "%s%02d.%s"; break;
  }
  sprintf(complete_filename, format, filename, k, extension);
}
/* -------------------------------------------------------------------------- */
void generate_path_filename(char *path, char *filename, char *complete_filename)
/* -------------------------------------------------------------------------- */
{
  sprintf(complete_filename, "%s%s", path, filename);
}
/* ----------------------------------------------------------------------------------------------------- */
void generate_path_filename_extension(char *path, char *filename, char *extension, char *complete_filename)
/* ----------------------------------------------------------------------------------------------------- */
{
  sprintf(complete_filename, "%s%s.%s", path, filename, extension);
}
/* -------------------------------------------------------------------------------------------------------------------- */
void generate_path_filename_sep_extension(char *path, char *filename, char *sep, char *extension, char *complete_filename)
/* -------------------------------------------------------------------------------------------------------------------- */
{
    sprintf(complete_filename, "%s%s%s.%s", path, filename, sep, extension);
}
/* --------------------------------------------------------------------------------------------------------------------------------- */
void generate_path_filename_k_ndigit_extension(char *path, char *filename, int k, int ndigit, char *extension, char *complete_filename)
/* --------------------------------------------------------------------------------------------------------------------------------- */
{
  char *format = "%s%s%d.%s";

  switch(ndigit) {
    case 0 : format = "%s%s%d.%s";   break;
    case 1 : format = "%s%s%01d.%s"; break;
    case 2 : format = "%s%s%02d.%s"; break;
    case 3 : format = "%s%s%03d.%s"; break;
    case 4 : format = "%s%s%04d.%s"; break;
    case 5 : format = "%s%s%05d.%s"; break;
    case 6 : format = "%s%s%06d.%s"; break;
    case 7 : format = "%s%s%07d.%s"; break;
    case 8 : format = "%s%s%08d.%s"; break;
    case 9 : format = "%s%s%09d.%s"; break;
    default: format = "%s%s%02d.%s"; break;
  }
  sprintf(complete_filename, format, path, filename, k, extension);
}
/* ------------------------------------------------------------------------------------------------------------------------------------------------ */
void generate_path_filename_sep_k_ndigit_extension(char *path, char *filename, char *sep, int k, int ndigit, char *extension, char *complete_filename)
/* ------------------------------------------------------------------------------------------------------------------------------------------------ */
{
    char *format = "%s%s%d.%s";
    
    switch(ndigit) {
        case 0 : format = "%s%s%s%d.%s";   break;
        case 1 : format = "%s%s%s%01d.%s"; break;
        case 2 : format = "%s%s%s%02d.%s"; break;
        case 3 : format = "%s%s%s%03d.%s"; break;
        case 4 : format = "%s%s%s%04d.%s"; break;
        case 5 : format = "%s%s%s%05d.%s"; break;
        case 6 : format = "%s%s%s%06d.%s"; break;
        case 7 : format = "%s%s%s%07d.%s"; break;
        case 8 : format = "%s%s%s%08d.%s"; break;
        case 9 : format = "%s%s%s%09d.%s"; break;
        default: format = "%s%s%s%02d.%s"; break;
    }
    sprintf(complete_filename, format, path, filename, sep, k, extension);
    
}
/* ------------------------------------------------------------------------------------------------------------------------------------------ */
void generate_path_filename_k_ndigit_l_extension(char *path, char *filename, int k, int ndigit, int l, char *extension, char *complete_filename)
/* ------------------------------------------------------------------------------------------------------------------------------------------ */
{
  char *format = "%s%s%d_%d.%s";

  switch(ndigit) {
    case 0 : format = "%s%s%d_%d.%s";   break;
    case 1 : format = "%s%s%01d_%01d.%s"; break;
    case 2 : format = "%s%s%02d_%02d.%s"; break;
    case 3 : format = "%s%s%03d_%03d.%s"; break;
    case 4 : format = "%s%s%04d_%04d.%s"; break;
    case 5 : format = "%s%s%05d_%05d.%s"; break;
    case 6 : format = "%s%s%06d_%06d.%s"; break;
    case 7 : format = "%s%s%07d_%07d.%s"; break;
    case 8 : format = "%s%s%08d_%08d.%s"; break;
    case 9 : format = "%s%s%09d_%09d.%s"; break;
    default: format = "%s%s%02d_%02d.%s"; break;
  }
  sprintf(complete_filename, format, path, filename, k, l, extension);
}
/* ------------------------------ */
float32* f32vector(int nl, int nh)
/* ------------------------------ */
{
  float32 *v;

  v=(float32 *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float32)));
  if (!v) nrerror("allocation failure in f32vector()");
  if(!v) return NULL;
  return v-nl+NR_END;
}

/* ------------------------------------------ */
uint8* ui8vector(int nl, int nh)
/* ------------------------------------------ */
/* allocate an uint8 vector with subscript range v[nl..nh] */
{
    uint8 *v;

    v=(uint8 *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(uint8)));
    if (!v) nrerror("allocation failure in ui8vector [%d..%d]", nl, nh);
    return v-nl+NR_END;
}
/* ------------------------------------------ */
uint16* ui16vector(int nl, int nh)
/* ------------------------------------------ */
/* allocate an uint16 vector with subscript range v[nl..nh] */
{
    uint16 *v;

    v=(uint16 *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(uint16)));
    if (!v) nrerror("allocation failure in ui16vector()");
    return v-nl+NR_END;
}

/* ------------------------------------------ */
uint32* ui32vector(int nl, int nh)
/* ------------------------------------------ */
/* allocate an uint8 vector with subscript range v[nl..nh] */
{
    uint32 *v;

    v=(uint32 *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(uint32)));
    if (!v) nrerror("allocation failure in ui32vector()");
    return v-nl+NR_END;
}
/* --------------------------------------------------- */
uint8** ui8matrix(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------- */
/* allocate an uint8 matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  uint8 **m;

  /* allocate pointers to rows */
  m=(uint8 **) malloc((size_t)((nrow+NR_END)*sizeof(uint8*)));
  if (!m) nrerror("allocation failure 1 in ui8matrix()");
  m += NR_END;
  m -= nrl;

  /* allocate rows and set pointers to them */
  m[nrl]=(uint8 *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(uint8)));
  if (!m[nrl]) nrerror("allocation failure 2 in ui8matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  /* return pointer to array of pointers to rows */
  return m;
}
/* --------------------------------------------------- */
sint8** si8matrix(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------- */
/* allocate an sint8 matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  sint8 **m;

  /* allocate pointers to rows */
  m=(sint8 **) malloc((size_t)((nrow+NR_END)*sizeof(sint8*)));
  if (!m) nrerror("allocation failure 1 in si8matrix()");
  m += NR_END;
  m -= nrl;

  /* allocate rows and set pointers to them */
  m[nrl]=(sint8 *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(sint8)));
  if (!m[nrl]) nrerror("allocation failure 2 in si8matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  /* return pointer to array of pointers to rows */
  return m;
}
/* --------------------------------------------------- */
uint16** ui16matrix(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------- */
/* allocate an uint16 matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  uint16 **m;

  /* allocate pointers to rows */
  m=(uint16 **) malloc((size_t)((nrow+NR_END)*sizeof(uint16*)));
  if (!m) nrerror("allocation failure 1 in ui16matrix()");
  m += NR_END;
  m -= nrl;

  /* allocate rows and set pointers to them */
  m[nrl]=(uint16 *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(uint16)));
  if (!m[nrl]) nrerror("allocation failure 2 in ui16matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  /* return pointer to array of pointers to rows */
  return m;
}
/* ----------------------------------------------------- */
sint16** si16matrix(int nrl, int nrh, int ncl, int nch)
/* ----------------------------------------------------- */
/* allocate an sint16 matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  sint16 **m;

  /* allocate pointers to rows */
  m=(sint16 **) malloc((size_t)((nrow+NR_END)*sizeof(sint16*)));
  if (!m) nrerror("allocation failure 1 in si16matrix()");
  m += NR_END;
  m -= nrl;

  /* allocate rows and set pointers to them */
  m[nrl]=(sint16 *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(sint16)));
  if (!m[nrl]) nrerror("allocation failure 2 in si16matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  /* return pointer to array of pointers to rows */
  return m;
}
/* --------------------------------------------------- */
uint32** ui32matrix(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------- */
/* allocate an uint32 matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  uint32 **m;

  /* allocate pointers to rows */
  m=(uint32 **) malloc((size_t)((nrow+NR_END)*sizeof(uint32*)));
  if (!m) nrerror("allocation failure 1 in ui16matrix()");
  m += NR_END;
  m -= nrl;

  /* allocate rows and set pointers to them */
  m[nrl]=(uint32 *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(uint32)));
  if (!m[nrl]) nrerror("allocation failure 2 in ui16matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  /* return pointer to array of pointers to rows */
  return m;
}
/* --------------------------------------------------- */
sint32** si32matrix(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------- */
/* allocate an sint32 matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  sint32 **m;

  /* allocate pointers to rows */
  m=(sint32 **) malloc((size_t)((nrow+NR_END)*sizeof(sint32*)));
  if (!m) nrerror("allocation failure 1 in ui16matrix()");
  m += NR_END;
  m -= nrl;

  /* allocate rows and set pointers to them */
  m[nrl]=(sint32 *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(sint32)));
  if (!m[nrl]) nrerror("allocation failure 2 in ui16matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  /* return pointer to array of pointers to rows */
  return m;
}
/* ----------------------------------------------------- */
float32** f32matrix(int nrl, int nrh, int ncl, int nch)
/* ----------------------------------------------------- */
/* allocate an float32 matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  float32 **m;

  /* allocate pointers to rows */
  m=(float32 **) malloc((size_t)((nrow+NR_END)*sizeof(float32*)));
  if (!m) nrerror("allocation failure 1 in f32matrix()");
  m += NR_END;
  m -= nrl;

  /* allocate rows and set pointers to them */
  m[nrl]=(float32 *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float32)));
  if (!m[nrl]) nrerror("allocation failure 2 in f32matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  /* return pointer to array of pointers to rows */
  return m;
}
/* --------------------------------------------------- */
rgb8** rgb8matrix(int nrl, int nrh, int ncl, int nch)
/* -------------------------------------------------- */
/* allocate an rgb8 matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  rgb8 **m;

  /* allocate pointers to rows */
  m=(rgb8**) malloc((size_t)((nrow+NR_END)*sizeof(rgb8*)));
  if (!m) nrerror("allocation failure 1 in rgb8matrix()");
  m += NR_END;
  m -= nrl;

  /* allocate rows and set pointers to them */
  m[nrl]=(rgb8*) malloc((size_t)((nrow*ncol+NR_END)*sizeof(rgb8)));
  if (!m[nrl]) nrerror("allocation failure 2 in rgb8matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  /* return pointer to array of pointers to rows */
  return m;
}
/* -------------------------------------------------- */
void dup_ui8vector(uint8 *X, int nl, int nh, uint8 *Y)
/* -------------------------------------------------- */
{
    int j;
    
    for(j=nl; j<=nh; j++) {
        Y[j] = X[j];
    }
}
/* -------------------------------------------------------------------------- */
void dup_ui8matrix(uint8 **X, int nrl, int nrh, int ncl, int nch, uint8 **Y)
/* -------------------------------------------------------------------------- */
{
    int i, j;

    for(i=nrl; i<=nrh; i++) {
        for(j=ncl; j<=nch; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* -------------------------------------------------------------------------- */
void dup_si8matrix(sint8 **X, int nrl, int nrh, int ncl, int nch, sint8 **Y)
/* -------------------------------------------------------------------------- */
{
    int i, j;

    for(i=nrl; i<=nrh; i++) {
        for(j=ncl; j<=nch; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* ----------------------------------------------------------------------------- */
void dup_ui16matrix(uint16 **X, int nrl, int nrh, int ncl, int nch, uint16 **Y)
/* ----------------------------------------------------------------------------- */
{
    int i, j;

    for(i=nrl; i<=nrh; i++) {
        for(j=ncl; j<=nch; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* ------------------------------------------------------------------------------ */
void dup_si16matrix(sint16 **X, int nrl, int nrh, int ncl, int nch, sint16  **Y)
/* ------------------------------------------------------------------------------ */
{
    int i, j;

    for(i=nrl; i<=nrh; i++) {
        for(j=ncl; j<=nch; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* ----------------------------------------------------------------------------- */
void dup_ui32matrix(uint32 **X, int nrl, int nrh, int ncl, int nch, uint32 **Y)
/* ----------------------------------------------------------------------------- */
{
    int i, j;

    for(i=nrl; i<=nrh; i++) {
        for(j=ncl; j<=nch; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* ----------------------------------------------------------------------------- */
void dup_si32matrix(sint32 **X, int nrl, int nrh, int ncl, int nch, sint32 **Y)
/* ----------------------------------------------------------------------------- */
{
    int i, j;

    for(i=nrl; i<=nrh; i++) {
        for(j=ncl; j<=nch; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* ------------------------------------------------------------------------------ */
void dup_f32matrix(float32 **X, int nrl, int nrh, int ncl, int nch, float32 **Y)
/* ------------------------------------------------------------------------------ */
{
    int i, j;

    for(i=nrl; i<=nrh; i++) {
        for(j=ncl; j<=nch; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
// --------------------------------------------------------
void extend_ui8vector(uint8 *X, int j0, int j1, int border)
// --------------------------------------------------------
{
    int j;
    
    // bord gauche et droit
    for(j=j0-border; j<=j0-1; j++) {
        X[j] = X[j0];
    }
    for(j=j1+1; j<=j1+border; j++) {
        X[j] = X[j1];
    }
}
/* ---------------------------------------------------------------------- */
void extend_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, int border)
/* ---------------------------------------------------------------------- */
{
    int i, j;

    // bord haut
    for(i=i0-border; i<=i0-1; i++) {
        for(j=j0; j<=j1; j++) {
            X[i][j] = X[i0][j];
        }
    }

    // bord bas
    for(i=i1+1; i<=i1+border; i++) {
        for(j=j0; j<=j1; j++) {
            X[i][j] = X[i1][j];
        }
    }

    // bord gauche et droit
    for(i=i0-border; i<=i1+border; i++) {
        
        for(j=j0-border; j<=j0-1; j++) {
            X[i][j] = X[i][j0];
        }
        for(j=j1+1; j<=j1+border; j++) {
            X[i][j] = X[i][j1];
        }
    }
}
/* ------------------------------------------------------------------------ */
void extend_ui16matrix(uint16 **X, int i0, int i1, int j0, int j1, int border)
/* ------------------------------------------------------------------------ */
{
    int i, j;

    // bord haut
    for(i=i0-border; i<i0; i++) {
        for(j=j0; j<=j1; j++) {
            X[i][j] = X[i0][j];
        }
    }

    // bord bas
    for(i=i1+1; i<=i1+border; i++) {
        for(j=j0; j<=j1; j++) {
            X[i][j] = X[i1][j];
        }
    }

    // bord gauche et droit
    for(i=i0-border; i<=i1+border; i++) {
        for(j=j0-border; j<j0; j++) {
            X[i][j] = X[i][j0];
        }
        for(j=j1+1; j<=j1+border; j++) {
            X[i][j] = X[i][j1];
        }
    }
}
/* ------------------------------------------- */
void free_f64vector(float64 *v, int nl, int nh)
/* ------------------------------------------- */
/* free a double vector allocated with f64vector() */
{
  free((FREE_ARG) (v+nl-NR_END));
}
/* ---------------------------------------------------------------- */
void free_ui8matrix(uint8 **m, int nrl, int nrh, int ncl, int nch)
/* ---------------------------------------------------------------- */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}
/* ---------------------------------------------------------------- */
void free_si8matrix(sint8 **m, int nrl, int nrh, int ncl, int nch)
/* ---------------------------------------------------------------- */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}
/* ------------------------------------------------------------------ */
void free_ui16matrix(uint16 **m, int nrl, int nrh, int ncl, int nch)
/* ------------------------------------------------------------------ */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}
/* ------------------------------------------------------------------ */
void free_si16matrix(sint16 **m, int nrl, int nrh, int ncl, int nch)
/* ------------------------------------------------------------------ */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}
/* ------------------------------------------------------------------ */
void free_ui32matrix(uint32 **m, int nrl, int nrh, int ncl, int nch)
/* ------------------------------------------------------------------ */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}
/* ------------------------------------------------------------------ */
void free_si32matrix(sint32 **m, int nrl, int nrh, int ncl, int nch)
/* ------------------------------------------------------------------ */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}
/* ------------------------------------------------------------------ */
void free_f32matrix(float32 **m, int nrl, int nrh, int ncl, int nch)
/* ------------------------------------------------------------------ */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}
/* ------------------------------------------------------- */
void free_ui8vector(uint8 *v, int nl, int nh)
/* ------------------------------------------------------- */
/* free a byte vector allocated with ui8vector() */
{
    free((FREE_ARG) (v+nl-NR_END));
}

/* ------------------------------------------------------- */
void free_ui32vector(uint32 *v, int nl, int nh)
/* ------------------------------------------------------- */
/* free a byte vector allocated with ui32vector() */
{
    free((FREE_ARG) (v+nl-NR_END));
}

/* ------------------------------------------------------- */
void free_f32vector(float32 *v, int nl, int nh)
/* ------------------------------------------------------- */
/* free a byte vector allocated with f32vector() */
{
    free((FREE_ARG) (v+nl-NR_END));
}

/* ---------------------------------------------------------------- */
void free_rgb8matrix(rgb8 **m, int nrl, int nrh, int ncl, int nch)
/* ---------------------------------------------------------------- */
/* free an uchar matrix allocated by rgb8matrix() */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}
/* ------------------------------------------------------- */
uint8** ui8matrix_map(int nrl, int nrh, int ncl, int nch)
/* ------------------------------------------------------- */
{
    int nrow=nrh-nrl+1;
    uint8 **m;

    /* allocate pointers to rows */
    m=(uint8 **) malloc((size_t)(nrow*sizeof(uint8*)));
    if (!m) nrerror("allocation failure 1 in ui8matrix_map()");
    m -= nrl;

    /* return pointer to array of pointers to rows */
    return m;
}
/* ------------------------------------------------------- */
sint8** si8matrix_map(int nrl, int nrh, int ncl, int nch)
/* ------------------------------------------------------- */
{
    int nrow=nrh-nrl+1;
    sint8 **m;

    /* allocate pointers to rows */
    m=(sint8 **) malloc((size_t)(nrow*sizeof(sint8*)));
    if (!m) nrerror("allocation failure 1 in si8matrix_map()");
    m -= nrl;

    /* return pointer to array of pointers to rows */
    return m;
}
/* --------------------------------------------------------- */
sint16** si16matrix_map(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------------- */
{
  int nrow=nrh-nrl+1;
  sint16 **m;

  /* allocate pointers to rows */
  m=(sint16 **) malloc((size_t)(nrow*sizeof(sint16*)));
  if (!m) nrerror("allocation failure 1 in si16matrix_map()");
  m -= nrl;

  /* return pointer to array of pointers to rows */
  return m;
}
/* --------------------------------------------------------- */
uint16** ui16matrix_map(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------------- */
{
    int nrow=nrh-nrl+1;
    uint16 **m;

    /* allocate pointers to rows */
    m=(uint16 **) malloc((size_t)(nrow*sizeof(uint16*)));
    if (!m) nrerror("allocation failure 1 in ui16matrix_map()");
    m -= nrl;

    /* return pointer to array of pointers to rows */
    return m;
}
/* --------------------------------------------------------- */
uint32** ui32matrix_map(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------------- */
{
    int nrow=nrh-nrl+1;
    uint32 **m;

    /* allocate pointers to rows */
    m=(uint32 **) malloc((size_t)(nrow*sizeof(uint32*)));
    if (!m) nrerror("allocation failure 1 in ui32matrix_map()");
    m -= nrl;

    /* return pointer to array of pointers to rows */
    return m;
}
/* --------------------------------------------------------- */
sint32** si32matrix_map(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------------- */
{
    int nrow=nrh-nrl+1;
    sint32 **m;

    /* allocate pointers to rows */
    m=(sint32 **) malloc((size_t)(nrow*sizeof(sint32*)));
    if (!m) nrerror("allocation failure 1 in si32matrix_map()");
    m -= nrl;

    /* return pointer to array of pointers to rows */
    return m;
}
/* --------------------------------------------------------- */
float32** f32matrix_map(int nrl, int nrh, int ncl, int nch)
/* --------------------------------------------------------- */
{
    int nrow=nrh-nrl+1;
    float32 **m;

    /* allocate pointers to rows */
    m=(float32 **) malloc((size_t)(nrow*sizeof(float32*)));
    if (!m) nrerror("allocation failure 1 in f32matrix_map()");
    m -= nrl;

    /* return pointer to array of pointers to rows */
    return m;
}
/* ------------------------------------------------------------------------------------------------------ */
sint8** si8matrix_map_1D_pitch(sint8 **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch)
/* ------------------------------------------------------------------------------------------------------ */
{
    int i;
    uint8 *p;

    /* map rows and set pointers to them */
    m[nrl]= (sint8*) data_1D;
    m[nrl] -= ncl;

    //for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1] + pitch;
    p = (uint8*) m[nrl];
    for(i=nrl+1;i<=nrh;i++) {
        p += pitch;
        m[i] = (sint8*) p;
    }

    /* return pointer to array of pointers to rows */
    return m;
}

/* ------------------------------------------------------------------------------------------------------ */
uint8** ui8matrix_map_1D_pitch(uint8 **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch)
/* ------------------------------------------------------------------------------------------------------ */
{
    int i;
    uint8 *p;

    /* map rows and set pointers to them */
    m[nrl]= (uint8*) data_1D;
    m[nrl] -= ncl;

    //for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1] + pitch;
    p = (uint8*) m[nrl];
    for(i=nrl+1;i<=nrh;i++) {
        p += pitch;
        m[i] = (uint8*) p;
    }

    /* return pointer to array of pointers to rows */
    return m;
}
/* --------------------------------------------------------------------------------------------------------- */
sint16** si16matrix_map_1D_pitch(sint16 **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch)
/* --------------------------------------------------------------------------------------------------------- */
{
    int i;
    uint8 *p;

    /* map rows and set pointers to them */
    m[nrl]= (sint16*) data_1D;
    m[nrl] -= ncl;

    //for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1] + pitch;
    p = (uint8*) m[nrl];
    for(i=nrl+1;i<=nrh;i++) {
        p += pitch;
        m[i] = (sint16*) p;
    }

    /* return pointer to array of pointers to rows */
    return m;
}

/* --------------------------------------------------------------------------------------------------------- */
uint16** ui16matrix_map_1D_pitch(uint16 **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch)
/* --------------------------------------------------------------------------------------------------------- */
{
    int i;
    uint8 *p;

    /* map rows and set pointers to them */
    m[nrl]= (uint16*) data_1D;
    m[nrl] -= ncl;

    //for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1] + pitch;
    p = (uint8*) m[nrl];
    for(i=nrl+1;i<=nrh;i++) {
        p += pitch;
        m[i] = (uint16*) p;
    }

    /* return pointer to array of pointers to rows */
    return m;
}
/* --------------------------------------------------------------------------------------------------------- */
sint32** si32matrix_map_1D_pitch(sint32 **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch)
/* --------------------------------------------------------------------------------------------------------- */
{
    int i;
    uint8 *p;

    /* map rows and set pointers to them */
    m[nrl]= (sint32*) data_1D;
    m[nrl] -= ncl;

    //for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1] + pitch;
    p = (uint8*) m[nrl];
    for(i=nrl+1;i<=nrh;i++) {
        p += pitch;
        m[i] = (sint32*) p;
    }

    /* return pointer to array of pointers to rows */
    return m;
}
/* --------------------------------------------------------------------------------------------------------- */
uint32** ui32matrix_map_1D_pitch(uint32 **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch)
/* --------------------------------------------------------------------------------------------------------- */
{
    int i;
    uint8 *p;

    /* map rows and set pointers to them */
    m[nrl]= (uint32*) data_1D;
    m[nrl] -= ncl;

    //for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1] + pitch;
    p = (uint8*) m[nrl];
    for(i=nrl+1;i<=nrh;i++) {
        p += pitch;
        m[i] = (uint32*) p;
    }

    /* return pointer to array of pointers to rows */
    return m;
}
/* ---------------------------------------------------------------------------------------------------------- */
float32** f32matrix_map_1D_pitch(float32 **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch)
/* ---------------------------------------------------------------------------------------------------------- */
{
    int i;
    uint8 *p;

    /* map rows and set pointers to them */
    m[nrl]= (float32*) data_1D;
    m[nrl] -= ncl;

    //for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1] + pitch;
    p = (uint8*) m[nrl];
    for(i=nrl+1;i<=nrh;i++) {
        p += pitch;
        m[i] = (float32*) p;
    }

    /* return pointer to array of pointers to rows */
    return m;
}
/* -------------------------------------------------------------------- */
void free_ui8matrix_map(uint8 **m, int nrl, int nrh, int ncl, int nch)
/* -------------------------------------------------------------------- */
{
    free((FREE_ARG)(m+nrl));
}
/* -------------------------------------------------------------------- */
void free_si8matrix_map(sint8 **m, int nrl, int nrh, int ncl, int nch)
/* -------------------------------------------------------------------- */
{
    free((FREE_ARG)(m+nrl));
}
/* ---------------------------------------------------------------------- */
void free_ui16matrix_map(uint16 **m, int nrl, int nrh, int ncl, int nch)
/* ---------------------------------------------------------------------- */
{
    free((FREE_ARG)(m+nrl));
}
/* ---------------------------------------------------------------------- */
void free_si16matrix_map(sint16 **m, int nrl, int nrh, int ncl, int nch)
/* ---------------------------------------------------------------------- */
{
    free((FREE_ARG)(m+nrl));
}
/* ---------------------------------------------------------------------- */
void free_ui32matrix_map(uint32 **m, int nrl, int nrh, int ncl, int nch)
/* ---------------------------------------------------------------------- */
{
    free((FREE_ARG)(m+nrl));
}
/* ---------------------------------------------------------------------- */
void free_si32matrix_map(sint32 **m, int nrl, int nrh, int ncl, int nch)
/* ---------------------------------------------------------------------- */
{
    free((FREE_ARG)(m+nrl));
}
/* ---------------------------------------------------------------------- */
void free_f32matrix_map(float32 **m, int nrl, int nrh, int ncl, int nch)
/* ---------------------------------------------------------------------- */
{
    free((FREE_ARG)(m+nrl));
}
/* --------------------------------------- */
void zero_ui8vector(uint8 *v, int i0, int i1)
/* --------------------------------------- */
{
    int i;
    for(i=i0; i<=i1; i++) {
        v[i] = 0;
    }
}
/* ----------------------------------------- */
void zero_ui32vector(uint32 *v, int i0, int i1)
/* ----------------------------------------- */
{
    int i;
    for(i=i0; i<=i1; i++) {
        v[i] = 0;
    }
}
/* -------------------------------------------------------- */
void zero_ui8matrix(uint8 **m, int i0, int i1, int j0, int j1)
/* -------------------------------------------------------- */
{
    int i, j;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            m[i][j] = 0;
        }
    }
}
/* ---------------------------------------------------------- */
void zero_ui16matrix(uint16 **m, int i0, int i1, int j0, int j1)
/* ---------------------------------------------------------- */
{
    int i, j;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            m[i][j] = 0;
        }
    }
}
/* ---------------------------------------------------------- */
void zero_f32matrix(float32 **m, int i0, int i1, int j0, int j1)
/* ---------------------------------------------------------- */
{
    int i, j;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            m[i][j] = 0;
        }
    }
}
/* -------------------------------------------------------- */
uint8 min_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1)
/* -------------------------------------------------------- */
{
    uint8 x, m=255;
    
    int i, j;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = X[i][j];
            if(x<m) m = x;
        }
    }
    return m;
}
/* -------------------------------------------------------- */
uint8 max_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1)
/* -------------------------------------------------------- */
{
    uint8 x, m=0;
    
    int i, j;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = X[i][j];
            if(x>m) m = x;
        }
    }
    return m;
}
/* -------------------------------------------------------------------- */
void init_ui8vector_param(uint8 *v, int j0, int j1, uint8 x0, uint8 xstep)
/* -------------------------------------------------------------------- */
{
    int j;
    uint8 x;
    
    x = x0;
    for(j=j0; j<=j1; j++) {
        v[j] = x;
        x += xstep;
    }
}
/* -------------------------------------------------------------------------- */
void init_f32vector_param(float32 *v, int j0, int j1, float32 x0, float32 xstep)
/* -------------------------------------------------------------------------- */
{
    int j;
    float32 x;

    x = x0;
    for(j=j0; j<=j1; j++) {
        v[j] = x;
        x += xstep;
    }
}
/* -------------------------------------------------------------------------------------------------- */
void init_ui8matrix_param(uint8 **m, int i0, int i1, int j0, int j1, uint8 x0, uint8 xstep, uint8 ystep)
/* -------------------------------------------------------------------------------------------------- */
{
    int i, j;
    uint8 x;
    
    x = x0;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            m[i][j] = x;
            x += xstep;
        }
        x += ystep;
    }
}
/* ---------------------------------------------------------------------------------------------------------- */
void init_f32matrix_param(float32 **m, int i0, int i1, int j0, int j1, float32 x0, float32 xstep, float32 ystep)
/* ---------------------------------------------------------------------------------------------------------- */
{
    int i, j;
    float32 x;

    x = x0;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            m[i][j] = x;
            x += xstep;
        }
        x += ystep;
    }
}
/* ----------------------------------------------------------------------------- */
void copy_ui8matrix_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
/* ----------------------------------------------------------------------------- */
{
    int i, j;

    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* ------------------------------------------------------------------------------- */
void copy_ui8matrix_ui16matrix(uint8 **X, int i0, int i1, int j0, int j1, uint16 **Y)
/* ------------------------------------------------------------------------------- */
{
    int i, j;

    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* ------------------------------------------------------------------------------- */
void copy_ui16matrix_ui8matrix(uint16 **X, int i0, int i1, int j0, int j1, uint8 **Y)
/* ------------------------------------------------------------------------------- */
{
    int i, j;

    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            Y[i][j] = (uint8) X[i][j];
        }
    }
}
/* ------------------------------------------------------------------------------- */
void copy_ui8matrix_f32matrix(uint8 **X, int i0, int i1, int j0, int j1, float32 **Y)
/* ------------------------------------------------------------------------------- */
{
    int i, j;

    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            Y[i][j] = X[i][j];
        }
    }
}
/* ------------------------------------------------------------------------------- */
void copy_f32matrix_ui8matrix(float32 **X, int i0, int i1, int j0, int j1, uint8 **Y)
/* ------------------------------------------------------------------------------- */
{
    int i, j;

    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            Y[i][j] = (uint8) X[i][j];
        }
    }
}

/* -------------------------------------------------------------------- */
void display_si8vector(sint8 *v,int nl,int nh, char *format, char *name)
/* -------------------------------------------------------------------- */
{
  int i;

  if(name != NULL) printf("%s", name);

  for(i=nl; i<=nh; i++) {
    printf(format, v[i]);
  }
  putchar('\n');
}
/* -------------------------------------------------------------------- */
void display_ui8vector(uint8 *v,int nl,int nh, char *format, char *name)
/* -------------------------------------------------------------------- */
{
  int i;

  if(name != NULL) printf("%s", name);

  for(i=nl; i<=nh; i++) {
    printf(format, v[i]);
  }
  putchar('\n');
}
/* ---------------------------------------------------------------------- */
void display_si16vector(sint16 *v,int nl,int nh, char *format, char *name)
/* ---------------------------------------------------------------------- */
{
  int i;

  if(name != NULL) printf("%s", name);

  for(i=nl; i<=nh; i++) {
    printf(format, v[i]);
  }
  putchar('\n');
}
/* ---------------------------------------------------------------------- */
void display_ui16vector(uint16 *v,int nl,int nh, char *format, char *name)
/* ---------------------------------------------------------------------- */
{
  int i;

  if(name != NULL) printf("%s", name);

  for(i=nl; i<=nh; i++) {
    printf(format, v[i]);
  }
  putchar('\n');
}
/* ---------------------------------------------------------------------- */
void display_si32vector(sint32 *v,int nl,int nh, char *format, char *name)
/* ---------------------------------------------------------------------- */
{
  int i;

  if(name != NULL) printf("%s", name);

  for(i=nl; i<=nh; i++) {
    printf(format, v[i]);
  }
  putchar('\n');
}
/* ---------------------------------------------------------------------- */
void display_ui32vector(uint32 *v,int nl,int nh, char *format, char *name)
/* ---------------------------------------------------------------------- */
{
  int i;

  if(name != NULL) printf("%s", name);

  for(i=nl; i<=nh; i++) {
    printf(format, v[i]);
  }
  putchar('\n');
}
/* ---------------------------------------------------------------------- */
void display_f32vector(float32 *v,int nl,int nh, char *format, char *name)
/* ---------------------------------------------------------------------- */
{
  int i;

  if(name != NULL) printf("%s", name);

  for(i=nl; i<=nh; i++) {
    printf(format, v[i]);
  }
  putchar('\n');
}
/* ------------------------------------------------------------------------------------------ */
void display_si8matrix(sint8 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name)
/* ------------------------------------------------------------------------------------------ */
{
  int i,j;

  if(name != NULL) puts(name);

  for(i=nrl; i<=nrh; i++) {
    for(j=ncl; j<=nch; j++) {
      printf(format, m[i][j]);
    }
    putchar('\n');
  }
}
/* ------------------------------------------------------------------------------------------ */
void display_ui8matrix(uint8 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name)
/* ------------------------------------------------------------------------------------------ */
{
  int i,j;

  if(name != NULL) puts(name);

  for(i=nrl; i<=nrh; i++) {
    for(j=ncl; j<=nch; j++) {
      printf(format, m[i][j]);
    }
    putchar('\n');
  }
}
/* -------------------------------------------------------------------------------------------- */
void display_si16matrix(sint16 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name)
/* -------------------------------------------------------------------------------------------- */
{
  int i,j;

  if(name != NULL) puts(name);

  for(i=nrl; i<=nrh; i++) {
    for(j=ncl; j<=nch; j++) {
      printf(format, m[i][j]);
    }
    putchar('\n');
  }
}
/* -------------------------------------------------------------------------------------------- */
void display_ui16matrix(uint16 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name)
/* -------------------------------------------------------------------------------------------- */
{
  int i,j;

  if(name != NULL) puts(name);

  for(i=nrl; i<=nrh; i++) {
    for(j=ncl; j<=nch; j++) {
      printf(format, m[i][j]);
    }
    putchar('\n');
  }
}
/* -------------------------------------------------------------------------------------------- */
void display_si32matrix(sint32 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name)
/* -------------------------------------------------------------------------------------------- */
{
  int i,j;

  if(name != NULL) puts(name);

  for(i=nrl; i<=nrh; i++) {
    for(j=ncl; j<=nch; j++) {
      printf(format, m[i][j]);
    }
    putchar('\n');
  }
}
/* -------------------------------------------------------------------------------------------- */
void display_ui32matrix(uint32 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name)
/* -------------------------------------------------------------------------------------------- */
{
  int i,j;

  if(name != NULL) puts(name);

  for(i=nrl; i<=nrh; i++) {
    for(j=ncl; j<=nch; j++) {
      printf(format, m[i][j]);
    }
    putchar('\n');
  }
}
/* -------------------------------------------------------------------------------------------- */
void display_f32matrix(float32 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name)
/* -------------------------------------------------------------------------------------------- */
{
  int i,j;

  if(name != NULL) puts(name);

  for(i=nrl; i<=nrh; i++) {
    for(j=ncl; j<=nch; j++) {
      printf(format, m[i][j]);
    }
    putchar('\n');
  }
}

/* --------------------------------------------------------------------------------------------- */
void display_ui8matrix_positive(uint8 **m, int i0, int i1, int j0, int j1, int iformat, char *name)
/* --------------------------------------------------------------------------------------------- */
{
    int i,j;

    char *format, *str;
    char *format1 = "%1d", *str1 = " ";
    char *format2 = "%2d", *str2 = "  ";
    char *format3 = "%3d", *str3 = "   ";
    char *format4 = "%4d", *str4 = "    ";
    
    switch(iformat) {
        case 1:  format = format1; str = str1; break;
        case 2:  format = format2; str = str2; break;
        case 3:  format = format3; str = str3; break;
        case 4:  format = format4; str = str4; break;
        default: format = format3; str = str3; break;
    }
        
    if(name != NULL) puts(name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            if(m[i][j]) {
                printf(format, m[i][j]);
            } else {
                printf("%s", str);
            }
        }
        putchar('\n');
    }    
}
/* ----------------------------------------------------------------------------------------------- */
void display_ui16matrix_positive(uint16 **m, int i0, int i1, int j0, int j1, int iformat, char *name)
/* ----------------------------------------------------------------------------------------------- */
{
    int i,j;
    
    char *format, *str;
    char *format1 = "%1d", *str1 = " ";
    char *format2 = "%2d", *str2 = "  ";
    char *format3 = "%3d", *str3 = "   ";
    char *format4 = "%4d", *str4 = "    ";
    
    switch(iformat) {
        case 1:  format = format1; str = str1; break;
        case 2:  format = format2; str = str2; break;
        case 3:  format = format3; str = str3; break;
        case 4:  format = format4; str = str4; break;
        default: format = format3; str = str3; break;
    }
    
    if(name != NULL) puts(name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            if(m[i][j]) {
                printf(format, m[i][j]);
            } else {
                printf("%s", str);
            }
        }
        putchar('\n');
    }    
}
/* ------------------------------------------------------------------------------ */
void display_ui16vector_number(uint16 *v, int j0, int j1, char *format, char *name)
/* ------------------------------------------------------------------------------ */
{
    int j;
    
    if(name != NULL) puts(name);
    for(j=j0; j<=j1; j++) {
        printf(format, j);
    }
    putchar('\n');
    
    for(j=j0; j<=j1; j++) {
        printf(format, v[j]);
    }
    putchar('\n');
}
/* ---------------------------------------------------------------------- */
void write_ui8vector(uint8 *v,int nl,int nh, char *format, char *filename)
/* ---------------------------------------------------------------------- */
{
    int  k;
    FILE *f;
    
    f = fopen(filename, "wt");
    if(f == NULL) { nrerror("Can't open file in write_ui8vector"); }
    for(k=nl; k<=nh; k++) { fprintf(f, format, v[k]); } fputc('\n', f);
    fclose(f);
}
/* ------------------------------------------------------------------------ */
void write_ui16vector(uint16 *v,int nl,int nh, char *format, char *filename)
/* ------------------------------------------------------------------------ */
{
    int  k;
    FILE *f;
    
    f = fopen(filename, "wt");
    if(f == NULL) { nrerror("Can't open file in write_ui16vector"); }
    for(k=nl; k<=nh; k++) { fprintf(f, format, v[k]); } fputc('\n', f);
    fclose(f);
}
/* ------------------------------------------------------------------------ */
void write_f32vector(float32 *v,int nl,int nh, char *format, char *filename)
/* ------------------------------------------------------------------------ */
{
    int  k;
    FILE *f;
    
    f = fopen(filename, "wt");
    if(f == NULL) { nrerror("Can't open file in write_f32vector"); }
    for(k=nl; k<=nh; k++) { fprintf(f, format, v[k]); } fputc('\n', f);
    fclose(f);
}
/* ------------------------ */
/* -- PGM IO for bmatrix -- */
/* ------------------------ */

//char *readitem   (FILE *file, char *buffer);
//void  ReadPGMrow (FILE *file, int width, uint8  *line);
//void  WritePGMrow(uint8 *line, int width, FILE  *file);

/* --------------------------------- */
char *readitem(FILE *file,char *buffer)
/* --------------------------------- */
/* lecture d'un mot */
{
  char *aux;
  int k;

  k=0;
  aux=buffer;
  while (!feof(file))
    {
      *aux=fgetc(file);
      switch(k)
        {
        case 0:
          if (*aux=='#') k=1;
          if (isalnum(*aux)) k=2,aux++;
          break;
        case 1:
          if (*aux==0xA) k=0;
          break;
        case 2:
          if (!isalnum(*aux))
            {
              *aux=0;
              return buffer;
            }
          aux++;
          break;
        }
    }
  *aux=0;
  return buffer;
}
/* ---------------------------------------------- */
void ReadPGMrow(FILE *file, int width, uint8  *line)
/* ---------------------------------------------- */
{
    /* Le fichier est ouvert (en lecture) et ne sera pas ferme a la fin */
     fread(&(line[0]), sizeof(uint8), width, file);
}
/* ----------------------------------------------- */
void WritePGMrow(uint8 *line, int width, FILE  *file)
/* ----------------------------------------------- */
{
/* Le fichier est deja ouvert et ne sera pas ferme a la fin */

   fwrite(&(line[0]), sizeof(uint8), width, file);
}
/* ------------------------------------------------------------------------------ */
uint8** LoadPGM_ui8matrix(char *filename, int *nrl, int *nrh, int *ncl, int *nch)
/* ------------------------------------------------------------------------------ */
{
  /* cette version ne lit plus que le type P5 */

  int height, width, gris;
  uint8 **m;
  FILE *file;
  /*int   format;/**/

  char *buffer;
  /*char  c;/**/
  int i;

  buffer = (char*) calloc(80, sizeof(char));
  /* ouverture du fichier */
  file = fopen(filename,"rb");
  if (file==NULL)
    nrerror("ouverture du fichier %s impossible\n", filename);

  /* lecture de l'entete du fichier pgm */
  readitem(file, buffer);
  /*fscanf(fichier, "%s", buffer);*/
  if(strcmp(buffer, "P5") != 0)
    nrerror("entete du fichier %s invalide\n", filename);

  width  = atoi(readitem(file, buffer));
  height = atoi(readitem(file, buffer));
  gris   = atoi(readitem(file, buffer));

  *nrl = 0;
  *nrh = height - 1;
  *ncl = 0;
  *nch = width - 1;
  m = ui8matrix(*nrl, *nrh, *ncl, *nch);

  for(i=0; i<height; i++) {
    ReadPGMrow(file, width, m[i]);
  }

  fclose(file);
  free(buffer);

  return m;
}
/* ------------------------------------------------------------------------------- */
void MLoadPGM_ui8matrix(char *filename, int nrl, int nrh, int ncl, int nch, uint8 **m)
/* ------------------------------------------------------------------------------- */
{
    /* cette version ne lit plus que le type P5 */

    int height, width, gris;
    FILE *file;

    char *buffer;
    int i;

    buffer = (char*) calloc(80, sizeof(char));
    /* ouverture du fichier */
    file = fopen(filename,"rb");
    if (file==NULL)
        nrerror("ouverture du fichier %s impossible", filename);

    /* lecture de l'entete du fichier pgm */
    readitem(file, buffer);
    /*fscanf(fichier, "%s", buffer);*/
    if(strcmp(buffer, "P5") != 0)
        nrerror("entete du fichier %s invalide", filename);

    width  = atoi(readitem(file, buffer));
    height = atoi(readitem(file, buffer));
    gris   = atoi(readitem(file, buffer));

    for(i=0; i<height; i++) {
        ReadPGMrow(file, width, m[i]);
    }

    fclose(file);
    free(buffer);
}
/* ----------------------------------------------------------------------------------- */
void SavePGM_ui8matrix(uint8 **m, int nrl, int nrh, int ncl, int nch, char *filename)
/* ----------------------------------------------------------------------------------- */
{
  int nrow = nrh-nrl+1;
  int ncol = nch-ncl+1;

  char buffer[80];

  FILE *file;
  int  i;

  file = fopen(filename, "wb");
  if (file == NULL)
    nrerror("ouverture du fichier %s impossible dans SavePGM_bmatrix", filename);

  /* enregistrement de l'image au format rpgm */

  sprintf(buffer,"P5\n%d %d\n255\n",ncol, nrow);
  fwrite(buffer,strlen(buffer),1,file);
  for(i=nrl; i<=nrh; i++)
    WritePGMrow(m[i], ncol, file);

  /* fermeture du fichier */
  fclose(file);
}
/* --------------------------- */
/* -- PNM IO for rgb8matrix -- */
/* --------------------------- */

/* ------------------------------------------------ */
void ReadPNMrow(FILE  *file, int width, uint8  *line)
/* ------------------------------------------------ */
{
    /* Le fichier est ouvert (en lecture) et ne sera pas ferme a la fin */
     fread(&(line[0]), sizeof(uint8), 3*sizeof(uint8)*width, file);
}
/* ------------------------------------------------ */
void WritePNMrow(uint8  *line, int width, FILE  *file)
/* ------------------------------------------------ */
{
/* Le fichier est deja ouvert et ne sera pas ferme a la fin */

   fwrite(&(line[0]), sizeof(uint8), 3*sizeof(uint8)*width, file);
}
/* ------------------------------------------------------------------------------- */
rgb8** LoadPPM_rgb8matrix(char *filename, int *nrl, int *nrh, int *ncl, int *nch)
/* ------------------------------------------------------------------------------- */
{
  /* cette version ne lit plus que le type P6 */

  int height, width, gris;
  rgb8 **m;
  FILE *file;
  /*int   format;*/

  char *buffer;
  /*char  c;/**/
  int i;

  buffer = (char*) calloc(80, sizeof(char));
  /* ouverture du fichier */
  file = fopen(filename,"rb");
  if (file==NULL)
    nrerror("ouverture du fichier %s impossible\n", filename);

  /* lecture de l'entete du fichier pgm */
  readitem(file, buffer);
  /*fscanf(fichier, "%s", buffer);*/
  if(strcmp(buffer, "P6") != 0)
    nrerror("entete du fichier %s invalide\n", filename);

  width  = atoi(readitem(file, buffer));
  height = atoi(readitem(file, buffer));
  gris   = atoi(readitem(file, buffer));

  *nrl = 0;
  *nrh = height - 1;
  *ncl = 0;
  *nch = width - 1;
  m = rgb8matrix(*nrl, *nrh, *ncl, *nch);

  for(i=0; i<height; i++) {
    ReadPNMrow(file, width, (uint8*)m[i]);
  }

  fclose(file);
  free(buffer);

  return m;
}
/* ----------------------------------------------------------------------------------- */
void SavePPM_rgb8matrix(rgb8 **m, int nrl, int nrh, int ncl, int nch, char *filename)
/* ----------------------------------------------------------------------------------- */
{
  int nrow = nrh-nrl+1;
  int ncol = nch-ncl+1;

  char buffer[80];

  FILE *file;
  int  i;

  file = fopen(filename, "wb");
  if (file == NULL)
    nrerror("ouverture du fichier %s impossible dans SavePPM_bmatrix\n", filename);

  /* enregistrement de l'image au format rpgm */

  sprintf(buffer,"P6\n%d %d\n255\n",ncol, nrow);
  fwrite(buffer,strlen(buffer),1,file);
  for(i=nrl; i<=nrh; i++)
    WritePNMrow((uint8*)m[i], ncol, file);

  /* fermeture du fichier */
  fclose(file);
}
