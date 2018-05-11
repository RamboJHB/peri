/* ------------------ */
/* --- sampleNR.h --- */
/* ------------------ */

/*
 * Copyright (c) 2011-2016 Lionel Lacassagne, all rights reserved
 * University Paris Sud 11
 * Univesity Pierre and Marie Curie
 */

#ifndef _SAMPLE_NR_H_
#define _SAMPLE_NR_H_

void generate_sample_constant(uint8 *X, int size);
void generate_sample_step_up(uint8 *X, int size, int amplitude);
void generate_sample_step_down(uint8 *X, int size, int amplitude);
void generate_sample_step_updown(uint8 *X, int size, int amplitude);

#endif // _SAMPLE_NR_H_