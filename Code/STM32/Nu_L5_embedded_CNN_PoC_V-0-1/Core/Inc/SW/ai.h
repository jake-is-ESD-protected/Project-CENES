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
#include "cnn.h"
#include "cnn_data.h"
}

#include "math.h"


#define AI_INPUT_RDY_FLAG	0x00010000
#define AI_OUTPUT_RDY_FLAG	0x00100000
#define AI_DATA_VERIFY_SEND	0x01000000
#define CEQ_MAX				5


/* [CLASS] cnn
 * @brief:	objectification of the convolutional neural network
 * @intent: access cnn init, inference & buffers under one banner
 * */
class cnn
{
public:

	uint8_t top3_class_idx[3] = {0};
	float top3_class_score[3] = {0};

	ai_float scale_buffer[AI_CNN_IN_1_HEIGHT][AI_CNN_IN_1_WIDTH] =
	{
		0//#include "ai_verify.txt"
	};

	ai_float out_data[AI_CNN_OUT_1_SIZE];

	ai_float out_fifo[CEQ_MAX][AI_CNN_OUT_1_SIZE] = {0};



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



	/* data normalizer
	 * @params:		`float pData[FPS][N_BANDS]` non scaled features (dBFS values)
	 * @params:		`float leq` equivalent level of duration of data
	 * @returns		`void`
	 * @brief:		normalize inputs by LEQ of the data
	 * @notes:		values get stored in the AI-input buffer
	 * */
	void normalize(float pData[FPS][N_BANDS], float leq);



	/* cnn-runner
	 * @params:		`void`
	 * @returns		`ai_float*`-pointer to output buffer of the cnn
	 * @brief:		run inference
	 * @notes:
	 * */
	ai_float* run();



private:

	ai_handle pNN = AI_HANDLE_NULL;

	ai_u8 activations[AI_CNN_DATA_ACTIVATIONS_SIZE];
	ai_float in_data[AI_CNN_IN_1_HEIGHT][AI_CNN_IN_1_WIDTH];

	ai_buffer *ai_input = NULL;
	ai_buffer *ai_output = NULL;

	ai_error err;
	ai_network_report report;

	ai_i32 n_batch;

};


extern cnn cnn_instance;

#endif
