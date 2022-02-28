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

	start_task(hRT_task, RT_task, (void*)this, &RT_task_attributes, &RT_task_running);
	start_task(hSW_task, SW_task, (void*)this, &SW_task_attributes, &SW_task_running);
	start_task(hUI_task, UI_task, (void*)this, &UI_task_attributes, &UI_task_running);

	cur_state = s_idle;
}



err core::handle(cmd c)
{

}



void core::RT_task(void* params)
{
	core* pCore = (core*) params;
	while(pCore->RT_task_running)
	{

	}
	osThreadTerminate(pCore->hRT_task);
}



void core::SW_task(void* params)
{
	core* pCore = (core*) params;
	while(pCore->SW_task_running)
	{

	}
	osThreadTerminate(pCore->hSW_task);
}



void core::UI_task(void* params)
{
	core* pCore = (core*) params;
	while(pCore->UI_task_running)
	{

	}
	osThreadTerminate(pCore->hUI_task);
}



void core::start_task(osThreadId_t hid, osThreadFunc_t f, void* p, const osThreadAttr_t* a, bool* run_flag)
{
	*run_flag = true;
	hid = osThreadNew(f, p, a);

	if(IS_NULL(hid))
	{
		*run_flag = false;
		err e(mem_null, MAIN, HIGH);
		e.handle();
	}
}
