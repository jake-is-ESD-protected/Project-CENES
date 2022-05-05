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
#include "ai.h"
#include "algorithm"
#include "app_fatfs.h"

#define PTR_SWAP(x, y) 	float* temp = x; x = y; y = temp;

#define STACK_WIDTH		256
#define STACK_SIZE_RT	(STACK_WIDTH * 8)
#define STACK_SIZE_SW	(STACK_WIDTH * 14)
#define STACK_SIZE_UI	(STACK_WIDTH * 10)

#define Z_WEIGHTING		0
#define A_WEIGHTING		1

#define CEQ_1S			1
#define CEQ_3S			3
#define CEQ_5S			5


class core
{
public:

	// Thread-handles
	osThreadId_t hRT_task = NULL;
	osThreadId_t hSW_task = NULL;
	osThreadId_t hUI_task = NULL;

	// Thread parameters
	const osThreadAttr_t RT_task_attributes = { // @suppress("Invalid arguments")
	  .name = "real-time task",
	  .stack_size = STACK_SIZE_RT,
	  .priority = (osPriority_t) osPriorityAboveNormal,
	};

	const osThreadAttr_t SW_task_attributes = { // @suppress("Invalid arguments")
	  .name = "AI & soft task",
	  .stack_size = STACK_SIZE_SW,
	  .priority = (osPriority_t) osPriorityNormal,
	};

	const osThreadAttr_t UI_task_attributes = { // @suppress("Invalid arguments")
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
		s_ai,
		s_settings
	}state_t;
	state_t cur_state = s_init;
	state_t last_state = s_init;

	// task control-flags
	bool RT_task_running = false;
	bool SW_task_running = false;
	bool UI_task_running = false;

	uint8_t weighting = Z_WEIGHTING;
	uint8_t ceq = CEQ_1S;
	uint8_t ceq_request = 1;
	uint8_t threshold_pcnt = 50;
	bool inference_request = false;


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



	void set_ceq(uint8_t ceq);



	void update_ceq(void);


private:


};
// singleton reference
extern core core_fsm;

#endif
