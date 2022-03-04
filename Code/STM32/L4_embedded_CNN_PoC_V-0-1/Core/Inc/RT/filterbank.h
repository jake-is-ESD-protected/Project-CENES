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

#define N_IIR_BIQUADS		(IIR_ORDER / 2)
#define N_DEC_IIR_BIQUADS	(DEC_IIR_ORDER / 2)
#define HALF_BUF_LEN		(FRAME_SIZE / 2)


/* [CLASS] filterbank
 * @brief:	the octave-filterbank as object
 * @intent:	have easy IO-operations regarding the filterbank
 * */
class filterbank
{
public:

	arm_biquad_casd_df1_inst_f32 iirsettings[N_BANDS], dec_iirsettings;
	float iir_state[N_BANDS][4*N_IIR_BIQUADS], dec_iir_state[4*N_DEC_IIR_BIQUADS];
	float input_buf[FRAME_SIZE / 2], output_buf[FRAME_SIZE / 2];




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
	 * @notes:		TODO
	 * */
	void init(void);



	/* filterbank-initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		init `arm_math`-specific structs
	 * @notes:		TODO
	 * */
	void run(void);
};


/* [CLASS] filter
 * @brief:	one octave-filter as object
 * @intent:	run filters based on index
 * */
class filter
{
public:

};



// singleton reference
extern filterbank fbank;

#endif
