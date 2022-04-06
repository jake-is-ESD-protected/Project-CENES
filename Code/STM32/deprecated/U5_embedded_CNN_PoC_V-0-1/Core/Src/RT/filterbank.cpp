/*
 * f_name:	filterbank.c
 * auth:	Jakob Tschavoll
 * ver:		1.4
 * brief:	source for filterbanks, decimations and weightings
 * */

#include "filterbank.h"
#include "coeffs.h"
#include "class_map.h"

// IIR-filter instance structs
arm_biquad_casd_df1_inst_f32 iirsettings[N_BANDS], dec_iirsettings;

// IIR-filter state buffers
float iir_state[N_BANDS][4*N_IIR_BIQUADS], dec_iir_state[4*N_DEC_IIR_BIQUADS];

// sample buffers
float toDSP_buf[FRAME_SIZE / 2], fromDSP_buf[FRAME_SIZE / 2];
uint16_t i;



void init_filters(void){

	// init filterbank
	for(uint8_t j = 0; j < N_BANDS; j++)
	{
		arm_biquad_cascade_df1_init_f32(&iirsettings[j], N_IIR_BIQUADS, &band_coeffs[j][0], &iir_state[j][0]);
	}

	// init decimator
	arm_biquad_cascade_df1_init_f32(&dec_iirsettings, N_DEC_IIR_BIQUADS, &dec_coeffs[0], &dec_iir_state[0]);


	// init A-weighting
	// TODO
}

void dsp(volatile uint32_t* _pIn, volatile uint32_t* _pOut){

//	for(i = 0; i < HALF_BUF_LEN; i++){
//
//		toDSP_buf[i] = (float)(((int32_t)_pIn[i])-(UINT12_MAX_VAL/2));
//	}
//
//	arm_biquad_cascade_df1_f32(	&iirsettings[N_IIR_FILTERS-1],
//								toDSP_buf,
//								fromDSP_buf,
//								HALF_BUF_LEN
//								);
//
//	for(i = 0; i < HALF_BUF_LEN; i++){
//
//		_pOut[i] = ((uint32_t)(fromDSP_buf[i] * scaler_abs + (UINT12_MAX_VAL/2)));
//	}
}


float rms(float* pBuf, uint16_t bufSize){
//	uint16_t i;
//	uint32_t sqr_sum = 0;
//	for(i = 0; i < bufSize; i++){
//		sqr_sum += (pow((uint32_t)(pBuf[i] / UINT12_MAX_VAL)), 2);
//	}
	return 0;

}


