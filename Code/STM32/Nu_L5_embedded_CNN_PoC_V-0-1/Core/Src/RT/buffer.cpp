/*
*****************************************************************************************
file name:       	buffer.cpp
auth:               Jakob Tschavoll
date:               22.02.22
brief:              buffers as objects and basic sampling operations
version:            V0.2
notes:
*****************************************************************************************
*/


#include <buffer.h>
#include "mailbox.h"
#include "gpio.h"
#include "core.h"


// singleton instance
buffer buf;



buffer::buffer(void)
{
	pOut = &mem[FRAME_SIZE];
}



void buffer::init(void)
{
	HAL_StatusTypeDef stat = HAL_SAI_DeInit(&hsai_BlockA1);
	stat = HAL_SAI_Init(&hsai_BlockA1);
	stat = HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*)mem, FRAME_SIZE * 2); // @suppress("Invalid arguments")
	is_init = true;
	if(stat != HAL_OK){
		is_init = false;
		e_handler.act(init_fail, buffer_e);
	}
}



void buffer::handle(void)
{

}



void buffer::start_sampler(void)
{
	active = true;
}



void buffer::stop_sampler(void)
{
	active = false;
}



void buffer::rx_hlf_cb(void* params)
{
	if(this->active)
	{
//		gpio_D3.set(true);
		pOut = &mem[0];
		osThreadFlagsSet(core_fsm.hRT_task, BUF_RDY_FLAG); // @suppress("Invalid arguments")
	}
}



void buffer::rx_full_cb(void* params)
{
	if(this->active)
	{
//		gpio_D3.set(false);
		pOut = &mem[FRAME_SIZE];
		osThreadFlagsSet(core_fsm.hRT_task, BUF_RDY_FLAG); // @suppress("Invalid arguments")
	}

}



/* half-full callback
 * @params: 	`SAI_HandleTypeDef* hsai`: SAI-handle
 * @returns 	`void`
 * @brief:		buffer is half full
 * @notes:		IMPLEMENTED BY HAL, CANNOT BE IN OBJECT
 * */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef* hsai)
{
	buf.rx_hlf_cb(NULL);
}




/* full callback
 * @params: 	`SAI_HandleTypeDef* hsai`: SAI-handle
 * @returns 	`void`
 * @brief:		buffer is full
 * @notes:		IMPLEMENTED BY HAL, CANNOT BE IN OBJECT
 * */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef* hsai)
{
	buf.rx_full_cb(NULL);
}
