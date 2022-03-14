/*
*****************************************************************************************
file name:       	core.h
auth:               Jakob Tschavoll
date:               25.02.22
brief:              Central FSM, connects UI with RT & SW
version:            V0.2
notes:
*****************************************************************************************
*/

#ifndef _CORE_H_
#define _CORE_H_


#include "main.h"
#include "cmsis_os.h"
#include "uterm.h"
#include "nextion.h"
#include "mailbox.h"
#include "filterbank.h"
#include "gpio.h"

#define STACK_WIDTH		128
#define STACK_SIZE_RT	(STACK_WIDTH * 4)
#define STACK_SIZE_SW	(STACK_WIDTH * 2)
#define STACK_SIZE_UI	(STACK_WIDTH * 10)


class core
{
public:

	// Thread-handles
	osThreadId_t hRT_task, hSW_task, hUI_task;

	// Thread parameters
	const osThreadAttr_t RT_task_attributes = {
	  .name = "real-time task",
	  .stack_size = STACK_SIZE_RT,
	  .priority = (osPriority_t) osPriorityNormal,
	};

	const osThreadAttr_t SW_task_attributes = {
	  .name = "AI & soft task",
	  .stack_size = STACK_SIZE_SW,
	  .priority = (osPriority_t) osPriorityNormal,
	};

	const osThreadAttr_t UI_task_attributes = {
	  .name = "user interface task",
	  .stack_size = STACK_SIZE_UI,
	  .priority = (osPriority_t) osPriorityNormal,
	};


	// possible states of operation
	typedef enum state
	{
		s_init,
		s_idle,
		s_rta,
		s_ai
	}state_t;
	state_t cur_state;
	state_t last_state;

	// task control-flags
	bool RT_task_running = false;
	bool SW_task_running = false;
	bool UI_task_running = false;


	/* core-constructor
	 * @params: 	`void`
	 * @returns 	`core`-object
	 * @brief:		create the central core object
	 * @notes:
	 * */
	core(void);



	/* core-initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		Init the core FSM
	 * @notes:
	 * */
	void init(void);



	/* command handler
	 * @params: 	`cmd` command to handle
	 * @returns 	`uint8_t`: status
	 * @brief:		handle an input command of generic origin
	 * @notes:
	 * */
	uint8_t handle(cmd c);



	/* real-time task
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		run all sampling and filtering applications
	 * @notes:
	 * */
	static void RT_task(void* params);



	/* soft-task task
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		run all AI and soft-applications
	 * @notes:
	 * */
	static void SW_task(void* params);



	/* user interface task
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		run all display, input and interfacing applications
	 * @notes:
	 * */
	static void UI_task(void* params);



	/* kickstart task
	 * @params:		`osThreadId_t* hid`: 	thread handle pointer
	 * @params:		`osThreadFunc_t f`: 	executing function
	 * @params:		`void* p`:				optional params
	 * @params:		`osThreadAttr_t* a`:	pointer to thread attribute struct
	 * @params:		`bool* run_flag`:		pointer to status flag
	 * @returns 	`void`
	 * @brief:		kickstart task and register its activity
	 * @notes:
	 * */
	void start_task(osThreadId_t* hid, osThreadFunc_t f, void* p, const osThreadAttr_t* a, bool* init_flag);


private:


};
// singleton reference
extern core core_fsm;

#endif
