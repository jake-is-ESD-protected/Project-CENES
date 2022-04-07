/*
*****************************************************************************************
file name:       	ai.h
auth:               Jakob Tschavoll
date:               16.03.22
brief:              neural network abstraction as object
version:            V0.1
notes:
*****************************************************************************************
*/


#ifndef _AI_H_
#define _AI_H_

#include "main.h"
#include "params.h"
#include "err.h"

extern "C"
{
#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "n4ced_v02.h"
#include "n4ced_v02_data.h"
}

#include "math.h"


#define AI_INPUT_RDY_FLAG	0x00010000
#define AI_OUTPUT_RDY_FLAG	0x00100000
#define AI_DATA_VERIFY_SEND	0x01000000


/* [CLASS] cnn
 * @brief:	objectification of the convolutional neural network
 * @intent: access cnn init, inference & buffers under one banner
 * */
class cnn
{
public:

	char class_map[7][20] = {
		"honk",
		"kids",
		"bark",
		"drill",
		"engine",
		"jackhammer",
		"music"
	};

	uint8_t top3_class_idx[3] = {0};
	float top3_class_score[3] = {0};

	ai_float scale_buffer[AI_N4CED_V02_IN_1_HEIGHT][AI_N4CED_V02_IN_1_WIDTH] =
	{
	#include "ai_verify.txt"
	};

	ai_float out_data[AI_N4CED_V02_OUT_1_SIZE];



	/* cnn-constructor
	 * @params:		`void`
	 * @returns:	`cnn`-object
	 * @brief:		Construct a neural network wrapper for underlying framework
	 * @notes:
	 * */
	cnn(void);



	/* cnn-initializer
	 * @params:		`void`
	 * @returns		`void`
	 * @brief:		init the neural network
	 * @notes:
	 * */
	void init();



	/* data scaler
	 * @params:		`float pData[FPS][N_BANDS]` non scaled features (dBFS values)
	 * @returns		`void`
	 * @brief:		scale inputs by statistical standardization to remove mean and ds
	 * @notes:		values get stored in the AI-input buffer
	 * */
	void scale_inputs(float pData[FPS][N_BANDS]);



	/* cnn-runner
	 * @params:		`void`
	 * @returns		`ai_float*`-pointer to output buffer of the cnn
	 * @brief:		run inference
	 * @notes:
	 * */
	ai_float* run();



private:

	ai_handle pNN = AI_HANDLE_NULL;

	ai_u8 activations[AI_N4CED_V02_DATA_ACTIVATIONS_SIZE];
	ai_float in_data[AI_N4CED_V02_IN_1_HEIGHT][AI_N4CED_V02_IN_1_WIDTH];

	ai_buffer *ai_input;
	ai_buffer *ai_output;

	ai_error err;
	ai_network_report report;

	ai_i32 n_batch;

};


extern cnn cnn_instance;

#endif
