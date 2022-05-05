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
A_filter a_weight;
ICS4343_correction ICS43432_correction_filter;



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

	for(uint8_t i = 0; i < N_DEC_STAGES; i++)
	{
		f_dec[i].init(&dec_coeffs[0]);
	}
}



float32_t filterbank::msqr2fs(float32_t msqr)
{
	return 10 * log10(msqr);
}



float32_t filterbank::rms2fs(float32_t rms)
{
	return 20 * log10(rms);
}



filter::filter(void)
{

}



void filter::init(float32_t* pCoeffs)
{
	arm_biquad_cascade_df1_init_f32(&iirsettings, N_IIR_BIQUADS, pCoeffs, delay_line_iir); // @suppress("Invalid arguments")
}



float32_t filter::run(float32_t* pSrc, float32_t* pDest, uint16_t n_samples)
{
	float32_t sqr_sum = 0;
	arm_biquad_cascade_df1_f32(&iirsettings, pSrc, pDest, n_samples); // @suppress("Invalid arguments")
	for(uint16_t i = 0; i < n_samples; i++)
	{
		sqr_sum += (pDest[i] * pDest[i]);
	}

	return sqr_sum;
}



void dec_filter::init(float32_t* coeffs)
{
	arm_biquad_cascade_df1_init_f32(&dec_iirsettings, N_DEC_IIR_BIQUADS, coeffs, delay_line_dec); // @suppress("Invalid arguments")
}



float32_t* dec_filter::run(float32_t* pData, uint16_t n_samples)
{
	arm_biquad_cascade_df1_f32(&dec_iirsettings, pData, temp_buf, n_samples); // @suppress("Invalid arguments")

	// decimate
	for(uint16_t i = 0; i < (n_samples/2); i++)
	{
		temp_buf[i] = temp_buf[i*2];
	}
	return temp_buf;
}



void A_filter::init(float32_t* coeffs)
{
	arm_biquad_cascade_df1_init_f32(&a_iirsettings, N_A_IIR_BIQUADS, coeffs, delay_line_a); // @suppress("Invalid arguments")
}



void A_filter::run(float32_t* pSrc, float32_t* pDest, uint16_t n_samples)
{
	arm_biquad_cascade_df1_f32(&a_iirsettings, pSrc, pDest, n_samples); // @suppress("Invalid arguments")
}



void ICS4343_correction::init(float32_t* coeffs)
{
	arm_biquad_cascade_df1_init_f32(&ICS43432_iirsettings, N_ICS43432_IIR_BIQUADS, coeffs, delay_line_ICS43432); // @suppress("Invalid arguments")
}



void ICS4343_correction::run(float32_t* pSrc, float32_t* pDest, uint16_t n_samples)
{
	arm_biquad_cascade_df1_f32(&ICS43432_iirsettings, pSrc, pDest, n_samples); // @suppress("Invalid arguments")
}
