/*
 * filterbank.c
 *
 *  Created on: 26.11.2021
 *      Author: Administrator
 */

#include "filterbank.h"
#include "coeffs.h"

//float scalings[] = {
//	#include "scalings.txt"
//};

float toDSP_buf[FRAME_SIZE / 2];
float fromDSP_buf[FRAME_SIZE / 2];
uint16_t i;
float scaler_abs; // let matlab do this

void init_filterbank(void){

	scaler_abs = pow(10, (scalings[3]/20));

	arm_biquad_cascade_df1_init_f32(&iirsettings[N_IIR_FILTERS-1],
									N_STAGES,
									&coeffs[3][0],
									&iir_state[N_IIR_FILTERS-3][0]
									);

	// init 30 IIR structures
//	for(uint8_t j = 0; j < N_IIR_FILTERS; j++)
//	{
//		arm_biquad_cascade_df1_init_f32(&iirsettings[j],
//										N_STAGES,
//										&coeffs[j][0],
//										&iir_state[j][0]
//										);
//	}
//
//	// init 10 decimators
//	for(uint8_t i = 0; i < N_FIR_DECIMATORS; i++)
//	{
//		arm_fir_decimate_init_f32(firdsettings[i], N_TAPS, (i), pCoeffs, pState, blockSize)
//	}
}

void dsp(volatile uint32_t* _pIn, volatile uint32_t* _pOut){

	for(i = 0; i < HALF_BUF_LEN; i++){

		toDSP_buf[i] = (float)(((int32_t)_pIn[i])-(UINT12_MAX_VAL/2));
	}

	arm_biquad_cascade_df1_f32(	&iirsettings[N_IIR_FILTERS-1],
								toDSP_buf,
								fromDSP_buf,
								HALF_BUF_LEN
								);

	for(i = 0; i < HALF_BUF_LEN; i++){

		_pOut[i] = ((uint32_t)(fromDSP_buf[i] * scaler_abs + (UINT12_MAX_VAL/2)));
	}
}

void feedthrough(volatile uint32_t* _pIn, volatile uint32_t* _pOut){

	for(i = 0; i < HALF_BUF_LEN; i++){
		_pOut[i] = _pIn[i];
	}
}

float rms(float* pBuf, uint16_t bufSize){
//	uint16_t i;
//	uint32_t sqr_sum = 0;
//	for(i = 0; i < bufSize; i++){
//		sqr_sum += (pow((uint32_t)(pBuf[i] / UINT12_MAX_VAL)), 2);
//	}
	return 0;

}


