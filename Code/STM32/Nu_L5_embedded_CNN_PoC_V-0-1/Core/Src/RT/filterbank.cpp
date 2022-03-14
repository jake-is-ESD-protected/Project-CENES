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
		f_array[N_BANDS - i - 1].pSrc = &input_buf[0];
		f_array[N_BANDS - i - 1].pDest = &output_buf[0];
		f_array[N_BANDS - i - 1].init(&band_coeffs[i][0]);

	}

	f_dec.init(&dec_coeffs[0]);
}



float32_t filterbank::msqr2spl(float32_t msqr)
{
	return 10 * log(msqr) + FS_TO_SPL_OFFS;
}



filter::filter(void)
{

}



void filter::init(float32_t* pCoeffs)
{
	arm_biquad_cascade_df1_init_f32(&iirsettings, N_IIR_BIQUADS, pCoeffs, delay_line_iir);
}



float32_t filter::run(float32_t* pSrc, float32_t* pDest, uint16_t n_samples)
{
	float32_t rms;
	arm_biquad_cascade_df1_f32(&iirsettings, pSrc, pDest, n_samples);
	arm_rms_f32(pDest, n_samples, &rms);

	return 20 * log10(rms); // max_val is 1, ergo no division
}



void dec_filter::init(float32_t* coeffs)
{
	arm_biquad_cascade_df1_init_f32(&dec_iirsettings, N_DEC_IIR_BIQUADS, coeffs, delay_line_dec);
}



float32_t* dec_filter::run(float32_t* pData, uint16_t n_samples)
{
	arm_biquad_cascade_df1_f32(&dec_iirsettings, pData, temp_buf, n_samples);

	// decimate
	for(uint16_t i = 0; i < (n_samples/2); i++)
	{
		temp_buf[i] = temp_buf[i*2];
	}
	return temp_buf;
}
