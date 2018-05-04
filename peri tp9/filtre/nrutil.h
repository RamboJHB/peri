/* ---------------- */
/* --- nrutil.h --- */
/* ---------------- */

/*
 * Copyright (c) 2011 Lionel Lacassagne, all rights reserved, University Paris Sud 11
 * Copyright NRC (www.nrc.com) Cambridge
 */

#ifndef __NRUTIL_H__
#define __NRUTIL_H__

#ifdef __cplusplus
//#pragma message ("C++")
extern "C" {
#endif

#include "def.h"

#define NR_END 0
#define FREE_ARG char*

extern long nr_end;

void nrerror(char *format, ...);
    
void generate_filename_k_ndigit                   (            char *filename,            int k, int ndigit,                         char *complete_filename);
void generate_filename_k_ndigit_extension         (            char *filename,            int k, int ndigit,        char *extension, char *complete_filename);
void generate_path_filename                       (char *path, char *filename,                                                       char *complete_filename);
void generate_path_filename_extension             (char *path, char *filename,                                      char *extension, char *complete_filename);
void generate_path_filename_sep_extension         (char *path, char *filename, char *sep,                           char *extension, char *complete_filename);
void generate_path_filename_k_ndigit_extension    (char *path, char *filename,            int k, int ndigit,        char *extension, char *complete_filename);
void generate_path_filename_sep_k_ndigit_extension(char *path, char *filename, char *sep, int k, int ndigit,        char *extension, char *complete_filename);
void generate_path_filename_k_ndigit_l_extension  (char *path, char *filename,            int k, int ndigit, int l, char *extension, char *complete_filename);
/* ------------------------------- */
/* -- vector & matrix allocator -- */
/* ------------------------------- */

uint8*   ui8vector(int nl, int nh);
uint16* ui16vector(int nl, int nh);
uint32*  ui32vector(int nl, int nh);
float32* f32vector(int nl, int nh);

void free_ui8vector(uint8 *v, int nl, int nh);
void free_ui32vector(uint32 *v, int nl, int nh);
void free_f32vector(float32 *v, int nl, int nh);

uint8**   ui8matrix(int nrl, int nrh, int ncl, int nch);
sint8**   si8matrix(int nrl, int nrh, int ncl, int nch);
uint16** ui16matrix(int nrl, int nrh, int ncl, int nch);
sint16** si16matrix(int nrl, int nrh, int ncl, int nch);
uint32** ui32matrix(int nrl, int nrh, int ncl, int nch);
sint32** si32matrix(int nrl, int nrh, int ncl, int nch);
float32** f32matrix(int nrl, int nrh, int ncl, int nch);

void dup_ui8vector(uint8 *X, int nl, int nh,uint8 *Y);
    
void dup_ui8matrix(uint8   **X, int nrl, int nrh, int ncl, int nch, uint8   **Y);
void dup_si8matrix(sint8   **X, int nrl, int nrh, int ncl, int nch, sint8   **Y);
void dup_ui16matrix(uint16 **X, int nrl, int nrh, int ncl, int nch, uint16  **Y);
void dup_si16matrix(sint16 **X, int nrl, int nrh, int ncl, int nch, sint16  **Y);
void dup_ui32matrix(uint32 **X, int nrl, int nrh, int ncl, int nch, uint32  **Y);
void dup_si32matrix(sint32 **X, int nrl, int nrh, int ncl, int nch, sint32  **Y);
void dup_f32matrix(float32 **X, int nrl, int nrh, int ncl, int nch, float32 **Y);

void extend_ui8vector(uint8  *X, int j0, int j1, int border);

void extend_ui8matrix (uint8  **X, int i0, int i1, int j0, int j1, int border);
void extend_ui16matrix(uint16 **X, int i0, int i1, int j0, int j1, int border);

void free_ui8matrix(uint8   **m, int nrl, int nrh, int ncl, int nch);
void free_si8matrix(sint8   **m, int nrl, int nrh, int ncl, int nch);
void free_ui16matrix(uint16 **m, int nrl, int nrh, int ncl, int nch);
void free_si16matrix(sint16 **m, int nrl, int nrh, int ncl, int nch);
void free_ui32matrix(uint32 **m, int nrl, int nrh, int ncl, int nch);
void free_si32matrix(sint32 **m, int nrl, int nrh, int ncl, int nch);
void free_f32matrix(float32 **m, int nrl, int nrh, int ncl, int nch);

uint8**   ui8matrix_map(int nrl, int nrh, int ncl, int nch);
sint8**   si8matrix_map(int nrl, int nrh, int ncl, int nch);
uint16** ui16matrix_map(int nrl, int nrh, int ncl, int nch);
sint16** si16matrix_map(int nrl, int nrh, int ncl, int nch);
uint32** ui23matrix_map(int nrl, int nrh, int ncl, int nch);
sint32** si32matrix_map(int nrl, int nrh, int ncl, int nch);
float32** f32matrix_map(int nrl, int nrh, int ncl, int nch);

uint8**    ui8matrix_map_1D_pitch(uint8   **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch);
sint8**    si8matrix_map_1D_pitch(sint8   **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch);
uint16**  ui16matrix_map_1D_pitch(uint16  **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch);
sint16**  si16matrix_map_1D_pitch(sint16  **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch);
uint32**  ui32matrix_map_1D_pitch(uint32  **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch);
sint32**  si32matrix_map_1D_pitch(sint32  **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch);
float32** f32matrix_map_1D_pitch (float32 **m, int nrl, int nrh, int ncl, int nch, void *data_1D, int pitch);

void free_ui8matrix_map (uint8  **m, int nrl, int nrh, int ncl, int nch);
void free_si8matrix_map (sint8  **m, int nrl, int nrh, int ncl, int nch);
void free_ui16matrix_map(uint16 **m, int nrl, int nrh, int ncl, int nch);
void free_si16matrix_map(sint16 **m, int nrl, int nrh, int ncl, int nch);
void free_ui32matrix_map(uint32 **m, int nrl, int nrh, int ncl, int nch);
void free_si32matrix_map(sint32 **m, int nrl, int nrh, int ncl, int nch);

/* ------------ */
/* --- zero --- */
/* ------------ */

void zero_ui8vector (uint8  *v, int i0, int i1);
void zero_ui32vector(uint32 *v, int i0, int i1);

void zero_ui8matrix(uint8 **m, int i0, int i1, int j0, int j1);
void zero_ui16matrix(uint16 **m, int i0, int i1, int j0, int j1);
void zero_f32matrix(float32 **m, int i0, int i1, int j0, int j1);

/* ----------- */
/* --- min --- */
/* ----------- */
uint8 min_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1);
uint8 max_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1);

/* ------------ */
/* --- init --- */
/* ------------ */
void init_ui8vector_param(uint8 *v,   int j0, int i1, uint8   x0, uint8   xstep);
void init_f32vector_param(float32 *v, int j0, int i1, float32 x0, float32 xstep);
    
void init_ui8matrix_param(uint8 **m,   int i0, int i1, int j0, int j1, uint8   x0, uint8   xstep, uint8   ystep);
void init_f32matrix_param(float32 **m, int i0, int i1, int j0, int j1, float32 x0, float32 xstep, float32 ystep);

/* ------------ */
/* --- copy --- */
/* ------------ */
    
void copy_ui8matrix_ui8matrix (uint8   **X, int i0, int i1, int j0, int j1, uint8   **Y);
void copy_ui8matrix_ui16matrix(uint8   **X, int i0, int i1, int j0, int j1, uint16  **Y);
void copy_ui16matrix_ui8matrix(uint16  **X, int i0, int i1, int j0, int j1, uint8   **Y);
void copy_ui8matrix_f32matrix (uint8   **X, int i0, int i1, int j0, int j1, float32 **Y);
void copy_f32matrix_ui8matrix (float32 **X, int i0, int i1, int j0, int j1, uint8   **Y);

/* ----------------------------- */
/* -- display vector & matrix -- */
/* ----------------------------- */

void display_si8vector (sint8  *v,int nl,int nh, char *format, char *name);
void display_ui8vector (uint8  *v,int nl,int nh, char *format, char *name);
void display_si16vector(sint16 *v,int nl,int nh, char *format, char *name);
void display_ui16vector(uint16 *v,int nl,int nh, char *format, char *name);
void display_si32vector(sint32 *v,int nl,int nh, char *format, char *name);
void display_ui32vector(uint32 *v,int nl,int nh, char *format, char *name);
void display_f32vector(float32 *v,int nl,int nh, char *format, char *name);

void display_ui16vector_number(uint16 *v,int j0, int j1, char *format, char *name);

void display_si8matrix (sint8  **m,int nrl,int nrh,int ncl, int nch, char *format, char *name);
void display_ui8matrix (uint8  **m,int nrl,int nrh,int ncl, int nch, char *format, char *name);
void display_si16matrix(sint16 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name);
void display_ui16matrix(uint16 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name);
void display_si32matrix(sint32 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name);
void display_ui32matrix(uint32 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name);
void display_f32matrix(float32 **m,int nrl,int nrh,int ncl, int nch, char *format, char *name);

void display_ui8matrix_positive (uint8  **m, int i0, int i1, int j0, int j1, int iformat, char *name);
void display_ui16matrix_positive(uint16 **m, int i0, int i1, int j0, int j1, int iformat, char *name);

/* -- write vector -- */
void write_ui8vector (uint8   *v,int nl,int nh, char *format, char *filename);
void write_ui16vector(uint16  *v,int nl,int nh, char *format, char *filename);
void write_f32vector (float32 *v,int nl,int nh, char *format, char *filename);
    
/* ------------- */
/* -- wrapper -- */
/* ------------- */

/* ------------------------------- */
/* -- PGM and PNM binary format -- */
/* ------------------------------- */

char *readitem(FILE *file,char *buffer);
void ReadPGMrow(FILE *file, int width, uint8  *line);
void WritePGMrow(uint8 *line, int width, FILE  *file);

uint8** LoadPGM_ui8matrix(char *filename, int *nrl, int *nrh, int *ncl, int *nch);
void   MLoadPGM_ui8matrix(char *filename, int nrl, int nrh, int ncl, int nch, uint8 **m);
void    SavePGM_ui8matrix(uint8 **m,       int  nrl, int  nrh, int  ncl, int  nch, char *filename);

rgb8 ** LoadPPM_rgb8matrix(char *filename, int *nrl, int *nrh, int *ncl, int *nch);
void    SavePPM_rgb8matrix(rgb8 **m,       int  nrl, int  nrh, int  ncl, int  nch, char *filename);

#ifdef __cplusplus
}
#endif

#endif // __NRUTL_H__
