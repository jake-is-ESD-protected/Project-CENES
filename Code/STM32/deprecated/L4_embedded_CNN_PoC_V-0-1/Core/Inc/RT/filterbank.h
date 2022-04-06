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

#define N_IIR_BIQUADS		(IIR_ORDER / 2)
#define N_DEC_IIR_BIQUADS	(DEC_IIR_ORDER / 2)
#define HALF_BUF_LEN		(FRAME_SIZE / 2)



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
	 * @brief:
	 * @notes:		TODO
	 * */
	filter(void);



	/* filter-initializer
	 * @params:		`float* coeffs`: pointer to coefficients
	 * @returns 	`void`
	 * @brief:
	 * @notes:		TODO
	 * */
	void init(float32_t* coeffs);



	/* filter-runner
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:
	 * @notes:		TODO
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
	 * @brief:
	 * @notes:		TODO
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
	float32_t temp_buf[FRAME_SIZE/2];
};



/* [CLASS] filterbank
 * @brief:	the octave-filterbank as object
 * @intent:	have easy IO-operations regarding the filterbank
 * */
class filterbank
{
public:

	float32_t input_buf[FRAME_SIZE / 2], output_buf[FRAME_SIZE / 2];
	filter f_array[N_BANDS];
	dec_filter f_dec;

	float32_t dbfs_buf[N_BANDS];


	/* filterbank-constructor
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:
	 * @notes:		TODO
	 * */
	filterbank(void);



	/* filterbank-initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		init `arm_math`-specific structs
	 * @notes:		orders the filters from fmax @index 0 to fmin @index N_BANDS
	 * */
	void init(void);



	/* filterbank-runner
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:
	 * @notes:		TODO
	 * */
	void run(void);
};


// singleton reference
extern filterbank fbank;

#endif
