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

	SW_task_running = false;
}



uint8_t core::handle(cmd c)
{
	uint8_t stat = 0;
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

	case uTerm_e:
		uTerm.handle(c);
		break;

	case nextion_e:
		// TODO
		break;

	case mailbox_e:
		// TODO
		break;

	case error_e:
		// TODO
		break;

	default:
		stat = -1;
		break;
	}
	return stat;
}



void core::RT_task(void* params)
{
	core* pCore = (core*) params;
	float32_t* pDec_data;
	uint8_t cnt = 0;

	buf.start_sampler();

	while(pCore->RT_task_running)
	{
		osThreadFlagsWait(BUF_RDY_FLAG, osFlagsWaitAny, osWaitForever);

		// begin time-measurement on scope
		gpio_D7.set(true);

		float msqr = 0;

		for(uint16_t i = 0; i < (FRAME_SIZE / 2); i++){

			fbank.input_buf[i] = ((float)(((int32_t)(buf.pOut[i] << 8)) >> 8)) / UINT24_MAX;
			msqr += (fbank.input_buf[i] * fbank.input_buf[i]);
		}

		fbank.lvl = fbank.msqr2spl(msqr);
		fbank.lvl_sum += fbank.lvl;


		uint8_t cur_band = 0;
		for(uint8_t i = 0; i < 4; i++)
		{
			fbank.dbfs_buf[cur_band] = fbank.f_array[cur_band].run(fbank.input_buf, fbank.output_buf, FRAME_SIZE / 2); cur_band++;
		}

		pDec_data = fbank.f_dec.run(fbank.input_buf, FRAME_SIZE / 2);
		for(uint8_t i = 0; i < 3; i++)
		{
			fbank.dbfs_buf[cur_band] = fbank.f_array[cur_band].run(pDec_data, fbank.output_buf, FRAME_SIZE / 4); cur_band++;
		}

		pDec_data = fbank.f_dec.run(pDec_data, FRAME_SIZE / 4);
		for(uint8_t i = 0; i < 3; i++)
		{
			fbank.dbfs_buf[cur_band] = fbank.f_array[cur_band].run(pDec_data, fbank.output_buf, FRAME_SIZE / 8); cur_band++;
		}

		pDec_data = fbank.f_dec.run(pDec_data, FRAME_SIZE / 8);
		for(uint8_t i = 0; i < 3; i++)
		{
			fbank.dbfs_buf[cur_band] = fbank.f_array[cur_band].run(pDec_data, fbank.output_buf, FRAME_SIZE / 16); cur_band++;
		}

		pDec_data = fbank.f_dec.run(pDec_data, FRAME_SIZE / 16);
		uint8_t remain = N_BANDS - cur_band;
		for(uint8_t i = 0; i < remain; i++)
		{
			fbank.dbfs_buf[cur_band] = fbank.f_array[cur_band].run(pDec_data, fbank.output_buf, FRAME_SIZE / 32); cur_band++;
		}

		cnt++;


		if((cnt % BIN_REFRESH_STEP) == 0)
		{
			for(uint8_t i = 0; i < N_BANDS; i++)
			{
				nextion.disp_data[i] = (uint8_t)(100. * ((fbank.dbfs_buf[i] + FS_TO_SPL_OFFS + (-1. * MIN_SPL_LVL_SHOWN)) / MAX_SPL_LVL_SHOWN));
			}
			osThreadFlagsSet(core_fsm.hUI_task, NEX_BUF_RDY_FLAG);
		}
		if((cnt % LVL_REFRESH_STEP) == 0)
		{
			cnt = 0;
			nextion.disp_lvl = fbank.lvl_sum / LVL_REFRESH_STEP;
			fbank.lvl_sum = 0;
			osThreadFlagsSet(core_fsm.hUI_task, NEX_LVL_RDY_FLAG);
		}

		gpio_D7.set(false);
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

	while(pCore->UI_task_running)
	{
		uint32_t flag = osThreadFlagsWait(INC_MSG_FLAG | NEX_BUF_RDY_FLAG | NEX_LVL_RDY_FLAG, osFlagsWaitAny, osWaitForever);

		switch(flag)
		{
		case INC_MSG_FLAG:
		{
			cmd c = uart_mbox.pop(false);
			uint8_t stat = pCore->handle(c);
			if(stat != 0)
			{
				// TODO err-handler
			}
			break;
		}
		case NEX_BUF_RDY_FLAG:
		{
			for(uint8_t i = 0; i < N_BANDS; i++)
			{
				nextion.tx(nextion.bin_id_buf[i], nextion.disp_data[i]);
			}
		}
		case NEX_LVL_RDY_FLAG:
		{
			nextion.tx("n0", nextion.disp_lvl);
		}
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
		// TODO err-handler
	}
}
