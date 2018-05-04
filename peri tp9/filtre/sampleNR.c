/* ------------------ */
/* --- sampleNR.c --- */
/* ------------------ */

/*
 * Copyright (c) 2011-2016 Lionel Lacassagne, all rights reserved
 * University Paris Sud 11
 * Univesity Pierre and Marie Curie
 */

#include <stdio.h>
#include <stddef.h>

#include "def.h"
#include "nrutil.h"

// ----------------------------------------------
void generate_sample_constant(uint8 *X, int size)
// ----------------------------------------------
{
	//signal constant de valeur
    //uint8 m = 128; // milieu
    uint8 m = 100; // milieu
    
    init_ui8vector_param(X, 0, size-1, m, 0);
}
// ------------------------------------------------------------
void generate_sample_step_up(uint8 *X, int size, int amplitude)
// ------------------------------------------------------------
{
    //uint8 m = 128; // milieu
    uint8 m = 100; // milieu
    uint8 m0 = m - amplitude / 2; // niveau bas
    uint8 m1 = m + amplitude / 2; // niveau haut
    
    init_ui8vector_param(X, 0,      size/2, m0, 0);
    init_ui8vector_param(X, size/2, size-1, m1, 0);
}
// --------------------------------------------------------------
void generate_sample_step_down(uint8 *X, int size, int amplitude)
// --------------------------------------------------------------
{
    //uint8 m = 128; // milieu
    uint8 m = 100; // milieu
    uint8 m0 = m - amplitude / 2; // niveau bas
    uint8 m1 = m + amplitude / 2; // niveau haut
    
    init_ui8vector_param(X, size/2, size-1, m1, 0);
    init_ui8vector_param(X, 0,      size/2, m0, 0);
}
// ----------------------------------------------------------------
void generate_sample_step_updown(uint8 *X, int size, int amplitude)//----echelon
// ----------------------------------------------------------------
{
    //uint8 m = 128; // milieu
    uint8 m = 100; // milieu
    uint8 m0 = m - amplitude / 2; // niveau bas
    uint8 m1 = m + amplitude / 2; // niveau haut
    
    init_ui8vector_param(X, 0,      size-1,     m0, 0);
    init_ui8vector_param(X, size/4, 3*size/4-1, m1, 0);
}
