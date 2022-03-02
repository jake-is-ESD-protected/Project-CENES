/*
 * f_name:	filterbank.h
 * auth:	Jakob Tschavoll
 * ver:		1.4
 * brief:	declarations for filterbanks, decimations and weightings
 * */

#ifndef _FILTERBANK_H_
#define _FILTERBANK_H_


#define ARM_MATH_CM4


#include "main.h"
#include "arm_math.h"
#include "params.h"
#include "sample.h"


#define N_IIR_BIQUADS		(IIR_ORDER / 2)
#define N_DEC_IIR_BIQUADS	(DEC_IIR_ORDER / 2)
#define HALF_BUF_LEN		(FRAME_SIZE / 2)

/*
 * init all filters (filterbank, anti-AA filters)
 */
void init_filters(void);


// run all filterbanks
void dsp(volatile uint32_t* _pIn, volatile uint32_t* _pOut);


// calc RMS for bins
float rms(float* pBuf, uint16_t bufSize);


#endif
