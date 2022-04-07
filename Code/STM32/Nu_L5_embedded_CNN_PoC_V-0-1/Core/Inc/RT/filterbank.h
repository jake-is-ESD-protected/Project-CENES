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

#ifndef _FILTERBANK_H_
#define _FILTERBANK_H_


#define ARM_MATH_CM4

#include "main.h"
#include "arm_math.h"
#include <buffer.h>
#include "params.h"

#define N_IIR_BIQUADS			(IIR_ORDER / 2)
#define N_DEC_IIR_BIQUADS		(DEC_IIR_ORDER / 2)
#define HALF_BUF_LEN			(FRAME_SIZE / 2)

#define SENSITIVITY_ICS43432	-26.
#define DB_SPL_94				94.
#define FS_TO_SPL_OFFS			(SENSITIVITY_ICS43432 * -1. + DB_SPL_94)



/* [CLASS] filter
 * @brief:	one octave-filter as object
 * @intent:	run filters based on index
 * */
class filter
{
public:

	float32_t* pSrc;
	float32_t* pDest;

	/* filter-constructor
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		construct an iir-bandpass
	 * @notes:
	 * */
	filter(void);



	/* filter-initializer
	 * @params:		`float* coeffs`: pointer to coefficients
	 * @returns 	`void`
	 * @brief:		`init the filter by specifying its sos-coefficients`
	 * @notes:
	 * */
	void init(float32_t* coeffs);



	/* filter-runner
	 * @params: 	`float32_t* pSrc` pointer to input
	 * @params: 	`float32_t* pDest` pointer to output
	 * @params: 	`uint16_t n_samples` number of samples in input
	 * @returns 	`void`
	 * @brief:		call the underlying ARM CMSIS DSP library to filter the signal
	 * @notes:
	 * */
	float32_t run(float32_t* pSrc, float32_t* pDest, uint16_t n_samples);


private:
	arm_biquad_casd_df1_inst_f32 iirsettings;
	float32_t delay_line_iir[4*N_IIR_BIQUADS];
};



/* [CLASS] dec_filter
 * @brief:	one decimation AA-filter as object
 * @intent:	derive from filter-class
 * */
class dec_filter: public filter
{
public:
	/* filter-initializer
	 * @params:		`float* coeffs`: pointer to coefficients
	 * @returns 	`void`
	 * @brief:		init a decimation filter
	 * @notes:		is a lowpass of high order
	 * */
	void init(float32_t* coeffs);


	/* filter-runner
	 * @params: 	`float32_t* pData`: pointer to data to be decimated
	 * @params:		`uint16_t n_samples`: number of samples to be filtered
	 * @returns 	`float32_t*`: pointer to decimated data
	 * @brief:
	 * @notes:		n_samples will be half as many after this function is done
	 * */
	float32_t* run(float32_t* pData, uint16_t n_samples);

private:
	arm_biquad_casd_df1_inst_f32 dec_iirsettings;
	float32_t delay_line_dec[4*N_DEC_IIR_BIQUADS];
	float32_t temp_buf[FRAME_SIZE];
};



/* [CLASS] filterbank
 * @brief:	the octave-filterbank as object
 * @intent:	have easy IO-operations regarding the filterbank
 * */
class filterbank
{
public:
	// IO-buffer
	float32_t input_buf[FRAME_SIZE], output_buf[FRAME_SIZE];
	// acting band of filters
	filter f_array[N_BANDS];
	// decimation-anti-AA filter
	dec_filter f_dec[N_DEC_STAGES];

	// bank output buffer (input to CNN)
	float32_t sqr_sum_buf[N_BANDS] = {0};

	uint8_t dec_map[28] = {1, 1, 1, 1, 2, 2, 2, 4, 4, 4, 8, 8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};


	/* filterbank-constructor
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		construct a filterbank which holds multiple filters
	 * @notes:
	 * */
	filterbank(void);



	/* filterbank-initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		init `arm_math`-specific structs
	 * @notes:		orders the filters from fmax @index 0 to fmin @index N_BANDS
	 * */
	void init(void);



	/* mean square to dBFS
	 * @params: 	`float32_t ms`: mean-square value
	 * @returns 	`float32_t`: dBFS
	 * @brief:		convert a mean-square value to an dBFS-value
	 * @notes:
	 * */
	float32_t msqr2fs(float32_t ms);




	/* root mean square to dBFS
	 * @params:		`float32_t rms` rms-value
	 * @returns:	`float32_t` dBFS-value (relative to 1)
	 * @brief:		convert an rms value to an dBFS-value
	 * @notes:
	 * */
	float32_t rms2fs(float32_t rms);
};


// singleton reference
extern filterbank fbank;

#endif
