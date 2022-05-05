/*
*****************************************************************************************
file name:       	buffer.h
auth:               Jakob Tschavoll
date:               22.02.22
brief:              buffers as objects and basic sampling operations
version:            V0.2
notes:
*****************************************************************************************
*/


#ifndef _SAMPLE_H_
#define _SAMPLE_H_


#define UINT24_MAX				16777215
#define NORM_FACTOR_U23_F32		8388608.
#define STACK_SIZE_CB			128
#define BUF_RDY_FLAG			0x00000001U

#define UINT_24					24
#define SIGN_EXT_MASK			(1u << (UINT_24 - 1))

#include "main.h"
#include "params.h"
#include "err.h"
#include <entry.h>
#include "cmsis_os2.h"


/* [CLASS] buffer
 * @brief:	memory buffers as objects
 * @intent:	unify memory, pointers and ready-flags within one class
 * */
class buffer
{
public:
	bool is_init 	= false;
	bool active 	= false;
	bool hlf_cplt 	= false;
	bool cplt 		= false;
	uint16_t size 	= FRAME_SIZE * 2;

	int32_t* pOut;

	osThreadId_t parent_thr = NULL;




	/* buffer-constructor
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		create object and set buffer pointers
	 * @notes:
	 * */
	buffer(void);



	/* buffer-initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		init buffer and pass a destination address for data
	 * @notes:
	 * */
	void init(void);



	/* buffer-handler
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		handle commands regarding buffer
	 * @notes:		NOT IMPLEMENTED
	 * */
	void handle(void);



	/* start sampler
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		activate the filling of buffer
	 * @notes:
	 * */
	void start_sampler(void);



	/* stop sampler
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		deactivate the filling of buffer
	 * @notes:
	 * */
	void stop_sampler(void);



	/* convert sample contents
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		converter
	 * @notes:		called by `sample()`
	 * */
	void fill_dest_buf(uint32_t* pInBuf, int32_t* pOutBuf, uint16_t len_out);



	/* abstraction for HAL_SAI_RxHalfCpltCallback()
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		buffer half full callback
	 * @notes:
	 * */
	void rx_hlf_cb(void* params);



	/* abstraction for HAL_SAI_RxCpltCallback()
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		buffer full callback
	 * @notes:
	 * */
	void rx_full_cb(void* params);


private:
	int32_t mem[FRAME_SIZE * 2];

};

// singleton reference
extern buffer buf;

#endif
