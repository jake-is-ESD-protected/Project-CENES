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

	uint8_t top_class_idx = 0;
	uint8_t top_class_score = 0;

	ai_float scale_buffer[AI_N4CED_V02_IN_1_HEIGHT][AI_N4CED_V02_IN_1_WIDTH] =
	{
	#include "ai_verify.txt"
	};

//	ai_float scale_buffer[AI_N4CED_V02_IN_1_HEIGHT][AI_N4CED_V02_IN_1_WIDTH];

	ai_float out_data[AI_N4CED_V02_OUT_1_SIZE];





	/* cnn-constructor
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:
	 * */
	cnn();



	/* TODO
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:
	 * */
	void init();



	/* TODO
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:
	 * */
	void scale_inputs(float pData[FPS][N_BANDS]);



	/* TODO
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:
	 * */
	ai_float* run();



	/* TODO
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:
	 * */
	uint8_t get_top_index(void);



	/* TODO
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:
	 * */
	void set_top_index(uint8_t idx);



	/* TODO
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:
	 * */
	uint8_t get_top_score(void);



	/* TODO
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:
	 * */
	void set_top_score(uint8_t score);

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
