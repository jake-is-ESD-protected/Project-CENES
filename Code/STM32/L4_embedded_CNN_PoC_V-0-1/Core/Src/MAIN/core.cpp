#include "core.h"


// singleton instance
core core_fsm;



core::core(void)
{

}



void core::init(void)
{
	cur_state = s_init;
	last_state = cur_state;

	start_task(&hRT_task, RT_task, (void*)this, &RT_task_attributes, &RT_task_running);
	start_task(&hSW_task, SW_task, (void*)this, &SW_task_attributes, &SW_task_running);
	start_task(&hUI_task, UI_task, (void*)this, &UI_task_attributes, &UI_task_running);
	cur_state = s_idle;
}



err core::handle(cmd c)
{
	err e = {.type = none, .origin = core_e, .prio = LOW, .description = NULL};
	switch(c.destination)
	{
	case core_e:
		// TODO
		break;

	case buffer_e:
		// TODO
		break;

	case filterbank_e:
		// TODO
		break;

	case ai_e:
		// TODO
		break;

	case little_bash_e:
		srl_uart.handle(c);
		break;

	case display_e:
		// TODO
		break;

	case mailbox_e:
		// TODO
		break;

	case error_e:
		// TODO
		break;

	default:
		e.type = no_such;
		e.prio = HIGH;
	}

	return e;
}



void core::RT_task(void* params)
{
	core* pCore = (core*) params;
	uint32_t rdy = 0;
	while(pCore->RT_task_running)
	{
		buf.start_sampler();
		rdy = osThreadFlagsWait(BUF_RDY_FLAG, osFlagsWaitAny, osWaitForever);

		if(rdy)
		{
			gpio_D7.set(true);

			for(uint16_t i = 0; i < (FRAME_SIZE / 2); i++){

				fbank.input_buf[i] = ((float)(((int32_t)(buf.pOut[i] << 8)) >> 8)) / UINT24_MAX;
			}
			gpio_D7.set(false);

			// run filterbank

		}
	}
	osThreadExit();
}



void core::SW_task(void* params)
{
	core* pCore = (core*) params;

	while(pCore->SW_task_running)
	{

	}
	osThreadExit();
}



void core::UI_task(void* params)
{
	core* pCore = (core*) params;
	err e = {.type = none, .origin = core_e, .prio = LOW, .description = NULL};

	while(pCore->UI_task_running)
	{
		while(uart_mbox.data_avail() == 0);
		cmd c = uart_mbox.pop(false);

		e = pCore->handle(c);
		if(e.type != none)
		{
			e.handle();
		}
	}
	osThreadExit();
}



void core::start_task(osThreadId_t* hTask, osThreadFunc_t f, void* p, const osThreadAttr_t* a, bool* init_flag)
{
	*init_flag = true;
	*hTask = osThreadNew(f, p, a);

	if(IS_NULL(*hTask))
	{
		err e(mem_null, core_e, HIGH, NULL);
		e.handle();
	}
}
