#ifndef _FILTERBANK_H_
#define _FILTERBANK_H_


#define ARM_MATH_CM4


#include "main.h"
#include "arm_math.h"
#include "params.h"


#define N_IIR_FILTERS		31
#define N_FIR_DECIMATORS	10
#define N_STAGES			3
#define N_BIQUADS			(N_IIR_FILTERS * N_STAGES)

#define FULL_BUF_LEN 		1024
#define HALF_BUF_LEN		FULL_BUF_LEN/2
#define UINT12_MAX_VAL		4095


// IIR-filter instance struct
arm_biquad_casd_df1_inst_f32 iirsettings[N_IIR_FILTERS];


// FIR-decimator instance struct
arm_fir_decimate_instance_f32 firdsettings[N_FIR_DECIMATORS];


// 4 delayed samples per biquad
float iir_state [N_IIR_FILTERS][4*N_STAGES];


// ? delayed samples per FIR
float fir_state [N_FIR_DECIMATORS][4]; // check how many are needed


// init all filterbanks
void init_filterbank(void);


// run all filterbanks
void dsp(volatile uint32_t* _pIn, volatile uint32_t* _pOut);


// feed values from adc to dac for testing
void feedthrough(volatile uint32_t* _pIn, volatile uint32_t* _pOut);


// calc RMS for bins
float rms(float* pBuf, uint16_t bufSize);


#endif
