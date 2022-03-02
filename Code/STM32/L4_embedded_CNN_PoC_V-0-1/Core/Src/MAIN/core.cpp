#include "core.h"
#include "mailbox.h"

// singleton instance
core core_fsm;



core::core(void)
{

}



void core::init(void)
{
	cur_state = s_init;
	last_state = cur_state;

	uart_mbox.init();

	start_task(hRT_task, RT_task, (void*)this, &RT_task_attributes, &RT_task_running);
	start_task(hSW_task, SW_task, (void*)this, &SW_task_attributes, &SW_task_running);
	start_task(hUI_task, UI_task, (void*)this, &UI_task_attributes, &UI_task_running);

	cur_state = s_idle;
}



err core::handle(cmd c)
{
	err e = {.type = none, .origin = core_e, .prio = LOW};

	if(c.destination == little_bash_e)
	{
		srl_uart.handle(c);
	}
	else
	{
		e.type = no_such;
		e.prio = HIGH;
	}

	return e;
}



void core::RT_task(void* params)
{
	core* pCore = (core*) params;
	while(pCore->RT_task_running)
	{

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

	err e = {.type = none, .origin = core_e, .prio = LOW};
	while(pCore->UI_task_running)
	{
		while(!uart_mbox.data_avail());
		cmd c = uart_mbox.pop(false);

		e = pCore->handle(c);
		if(e.type != none)
		{
			e.handle();
		}
	}
	osThreadExit();
}



void core::start_task(osThreadId_t hTask, osThreadFunc_t f, void* p, const osThreadAttr_t* a, bool* run_flag)
{
	*run_flag = true;
	hTask = osThreadNew(f, p, a);

	if(IS_NULL(hTask))
	{
		*run_flag = false;
		err e(mem_null, core_e, HIGH);
		e.handle();
	}
}
