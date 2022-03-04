/*
*****************************************************************************************
file name:       	filterbank.cpp
auth:               Jakob Tschavoll
date:               03.03.22
brief:              source for filterbanks, decimations and weightings
version:            V0.4
notes:
*****************************************************************************************
*/

#include "filterbank.h"
#include "coeffs.h"


// singleton instance
filterbank fbank;



filterbank::filterbank(void)
{

}



void filterbank::init(void)
{
	for(uint8_t i = 0; i < N_BANDS; i++)
	{
		arm_biquad_cascade_df1_init_f32(&iirsettings[i],
										N_IIR_BIQUADS,
										&band_coeffs[i][0],
										&iir_state[i][0]);
	}

	arm_biquad_cascade_df1_init_f32(&dec_iirsettings,
									N_DEC_IIR_BIQUADS,
									&dec_coeffs[0],
									&dec_iir_state[0]);
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


