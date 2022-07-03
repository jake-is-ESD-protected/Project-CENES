#include "core.h"
#include <stdio.h>
#include <vector>
#include <numeric>
#include "class_map.h"

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
}



uint8_t core::handle(cmd c)
{
	uint8_t stat = 0;
	switch(c.destination)
	{	// TODO: expand core-commands (if needed)
	case core_e:
		break;

	case buffer_e:
		break;

	case filterbank_e:
		break;

	case ai_e:
		break;

	case uTerm_e:
		uTerm.handle(c);
		break;

	case nextion_e:
		nextion.handle(c);
		break;

	case mailbox_e:
		break;

	case error_e:
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
	float sqr_sum = 0;

	buf.start_sampler();

	while(pCore->RT_task_running)
	{

		// wait for i2s buffer to be full
		osThreadFlagsWait(BUF_RDY_FLAG, osFlagsWaitAny, osWaitForever); // @suppress("Invalid arguments")


#ifdef MEAS_SETUP
		// begin time-measurement on scope
		gpio_D3.set(true);
#endif


		// align data from i2s-buffer & convert it to float between 1 & -1
		for(uint16_t i = 0; i < (FRAME_SIZE); i++){
			fbank.iBuf[i] = (((buf.pOut[i]) << 8) / 256) / NORM_FACTOR_U23_F32;
		}

#ifdef ICS43432_CORRECTION
		// apply ICS43432-correction-curve
		ICS43432_correction_filter.run(fbank.iBuf, fbank.oBuf, FRAME_SIZE);
		PTR_SWAP(fbank.iBuf, fbank.oBuf);
#endif

		// apply A-weighting:
		if(pCore->weighting == A_WEIGHTING || pCore->cur_state == s_ai)
		{
			a_weight.run(fbank.iBuf, fbank.oBuf, FRAME_SIZE);
			PTR_SWAP(fbank.iBuf, fbank.oBuf)
		}


		// calc LEQ
		float short_sqr_sum = 0;
		for(uint16_t i = 0; i < (FRAME_SIZE); i++){
			sqr_sum += (fbank.iBuf[i] * fbank.iBuf[i]);
			short_sqr_sum = sqr_sum;	// for later implementation of frame-triggered classification
		}
		if(sqr_sum == 0) 		// microphone error
		{
			gpio_D2.set(true);	// microphone dead
		}
		else
		{
			gpio_D2.set(false);
		}



		// filter-stages:
		uint8_t cur_band = 0;
		float cur_sqr_sum = 0;
		for(uint8_t i = 0; i < 4; i++)
		{
			cur_sqr_sum = fbank.f_array[cur_band].run(fbank.iBuf, fbank.oBuf, FRAME_SIZE);
			fbank.sqr_sum_buf[cur_band] += cur_sqr_sum;
			cnn_instance.scale_buffer[cnt][cur_band] = fbank.msqr2fs(cur_sqr_sum / FRAME_SIZE);
			cur_band++;
		}


		pDec_data = fbank.f_dec[0].run(fbank.iBuf, FRAME_SIZE);
		for(uint8_t i = 0; i < 3; i++)
		{
			cur_sqr_sum = fbank.f_array[cur_band].run(pDec_data, fbank.oBuf, FRAME_SIZE / fbank.dec_map[cur_band]);
			fbank.sqr_sum_buf[cur_band] += cur_sqr_sum;
			cnn_instance.scale_buffer[cnt][cur_band] = fbank.msqr2fs(cur_sqr_sum / (FRAME_SIZE / fbank.dec_map[cur_band]));
			cur_band++;
		}


		pDec_data = fbank.f_dec[1].run(pDec_data, FRAME_SIZE / 2);
		for(uint8_t i = 0; i < 3; i++)
		{
			cur_sqr_sum = fbank.f_array[cur_band].run(pDec_data, fbank.oBuf, FRAME_SIZE / fbank.dec_map[cur_band]);
			fbank.sqr_sum_buf[cur_band] += cur_sqr_sum;
			cnn_instance.scale_buffer[cnt][cur_band] = fbank.msqr2fs(cur_sqr_sum / (FRAME_SIZE / fbank.dec_map[cur_band]));
			cur_band++;
		}


		pDec_data = fbank.f_dec[2].run(pDec_data, FRAME_SIZE / 4);
		for(uint8_t i = 0; i < 3; i++)
		{
			cur_sqr_sum = fbank.f_array[cur_band].run(pDec_data, fbank.oBuf, FRAME_SIZE / fbank.dec_map[cur_band]);
			fbank.sqr_sum_buf[cur_band] += cur_sqr_sum;
			cnn_instance.scale_buffer[cnt][cur_band] = fbank.msqr2fs(cur_sqr_sum / (FRAME_SIZE / fbank.dec_map[cur_band]));
			cur_band++;
		}


		pDec_data = fbank.f_dec[3].run(pDec_data, FRAME_SIZE / 8);
		uint8_t remain = N_BANDS - cur_band;
		for(uint8_t i = 0; i < remain; i++)
		{
			cur_sqr_sum = fbank.f_array[cur_band].run(pDec_data, fbank.oBuf, FRAME_SIZE / fbank.dec_map[cur_band]);
			fbank.sqr_sum_buf[cur_band] += cur_sqr_sum;
			cnn_instance.scale_buffer[cnt][cur_band] = fbank.msqr2fs(cur_sqr_sum / (FRAME_SIZE / fbank.dec_map[cur_band]));
			cur_band++;
		}


		cnt++;


		// fill display buffer for bins and send update to nextion (UI-task)
		if((cnt % BIN_REFRESH_STEP) == 0 && (pCore->cur_state == s_rta))
		{
			for(uint8_t i = 0; i < N_BANDS; i++)
			{
				float bin_spl = fbank.msqr2fs(fbank.sqr_sum_buf[i] / (BIN_REFRESH_STEP * (FRAME_SIZE / fbank.dec_map[i]))) + FS_TO_SPL_OFFS;
				fbank.sqr_sum_buf[i] = 0;
				float bin_prcnt = bin_spl + (MIN_SPL_LVL_SHOWN * (-1));
				bin_prcnt /= ((MIN_SPL_LVL_SHOWN * (-1)) + MAX_SPL_LVL_SHOWN);
				if(bin_prcnt < 0) bin_prcnt = 0;	// catch visual wraparound
				if(bin_prcnt > 1) bin_prcnt = 1;	// catch visual clipping
				nextion.disp_data[i] = (uint8_t)(100. * bin_prcnt);

			}
			osThreadFlagsSet(core_fsm.hUI_task, NEX_BUF_RDY_FLAG); // @suppress("Invalid arguments")
		}


		// fill display buffer for SPL-level and send update to nextion (UI-task)
		if((cnt % LVL_REFRESH_STEP) == 0 && (pCore->cur_state == s_rta))
		{
			nextion.disp_lvl = fbank.msqr2fs(sqr_sum / (FRAME_SIZE * LVL_REFRESH_STEP)) + FS_TO_SPL_OFFS;
			sqr_sum = 0;
			osThreadFlagsSet(core_fsm.hUI_task, NEX_LVL_RDY_FLAG); // @suppress("Invalid arguments")
		}

		// call CNN to classify
		if(cnt == FPS && (pCore->cur_state == s_ai))
		{
			if(pCore->inference_request)
			{
				float leq = fbank.msqr2fs(sqr_sum / (FRAME_SIZE * FPS));
				cnn_instance.normalize(cnn_instance.scale_buffer, leq);
				osThreadFlagsSet(core_fsm.hSW_task, AI_INPUT_RDY_FLAG); // @suppress("Invalid arguments")
			}
			else
			{
				cmd c;
				c.type = reset_classes;
				c.origin = core_e;
				c.destination = nextion_e;
				uart_mbox.push(c, MBOX_TIMEOUT, core_fsm.hUI_task, INC_MSG_FLAG);
			}
		}

		if(cnt == LVL_REFRESH_STEP && (pCore->cur_state == s_ai))
		{
			float spl = fbank.msqr2fs(sqr_sum / (LVL_REFRESH_STEP * FRAME_SIZE)) + FS_TO_SPL_OFFS;
			sqr_sum = 0;
			float bin_prcnt = spl + (MIN_SPL_LVL_SHOWN * (-1));
			bin_prcnt /= ((MIN_SPL_LVL_SHOWN * (-1)) + MAX_SPL_LVL_SHOWN);
			if(bin_prcnt < 0) bin_prcnt = 0;	// catch visual wraparound
			if(bin_prcnt > 1) bin_prcnt = 1;	// catch visual clipping
			nextion.disp_lvl = (uint8_t)(100. * bin_prcnt);

			if(nextion.disp_lvl > pCore->threshold_pcnt)
			{
				pCore->inference_request = true;
			}
			else
			{
				pCore->inference_request = false;
			}

			osThreadFlagsSet(core_fsm.hUI_task, NEX_LVL_RDY_FLAG); // @suppress("Invalid arguments")
		}


		// common cnt-reset
		if(cnt == FPS) cnt = 0;


#ifdef MEAS_SETUP
		// end time-measurement on scope
		gpio_D3.set(false);
#endif

	}
	osThreadExit();
}



void core::SW_task(void* params)
{
	core* pCore = (core*) params;
	uint8_t cnt = 0;

	while(pCore->SW_task_running)
	{
		// wait for flag
		osThreadFlagsWait(AI_INPUT_RDY_FLAG, osFlagsWaitAny, osWaitForever); // @suppress("Invalid arguments")


		// start time-measurement on scope
		gpio_D4.set(true);

		// run inference
		cnn_instance.run();

		// end time-measurement on scope
		gpio_D4.set(false);


		for(uint8_t i = 0; i < AI_CNN_OUT_1_SIZE; i++)
		{
			cnn_instance.out_fifo[cnt][i] = cnn_instance.out_data[i];
		}

		cnt++;

		// get mean of class over time: CEQ
		for(uint8_t i = 0; i < AI_CNN_OUT_1_SIZE; i++)
		{
			float class_mean = 0;
			for(uint8_t j = 0; j < pCore->ceq; j++)
			{
				class_mean += cnn_instance.out_fifo[j][i];
			}
			cnn_instance.out_data[i] = class_mean / pCore->ceq;
		}

#ifdef TEST_SETUP
		// send output tensor via uterm to pc
		uTerm.printf(	"%f %f %f %f %f\r\n",
						cnn_instance.out_data[0],
						cnn_instance.out_data[1],
						cnn_instance.out_data[2],
						cnn_instance.out_data[3],
						cnn_instance.out_data[4]);
#endif



		// sort vector (magic)
		std::vector<float> a(std::begin(cnn_instance.out_data), std::end(cnn_instance.out_data));
		std::vector<int> v(AI_CNN_OUT_1_SIZE);
		std::iota(v.begin(),v.end(),0);
		sort(v.begin(), v.end(), [&](int i,int j){return a[i]>=a[j];});

		// get top 3 classes + scores
		for(uint8_t i = 0; i < 3; i++)
		{
			cnn_instance.top3_class_idx[i] = v[i];
			cnn_instance.top3_class_score[i] = cnn_instance.out_data[v[i]];
		}

		if(cnt == pCore->ceq)
		{
			cnt = 0;
			// at the end of each buffer, check if the user changed CEQ
			pCore->update_ceq();
		}

		osThreadFlagsSet(pCore->hUI_task, AI_OUTPUT_RDY_FLAG); // @suppress("Invalid arguments")



	}
	osThreadExit();
}



void core::UI_task(void* params)
{
	core* pCore = (core*) params;

	bool inc_msg;
	bool buf_rdy;
	bool lvl_rdy;
	bool cnn_rdy;

	cmd c = {.type=switch_rta, .origin=core_e, .destination=nextion_e, .prio=HIGH, .params=NULL};
	pCore->handle(c);

	while(pCore->UI_task_running)
	{
		uint32_t flag = osThreadFlagsWait(	INC_MSG_FLAG | NEX_BUF_RDY_FLAG | NEX_LVL_RDY_FLAG | AI_OUTPUT_RDY_FLAG, // @suppress("Invalid arguments")
											osFlagsWaitAny,
											osWaitForever);

		inc_msg = ((flag & INC_MSG_FLAG) == INC_MSG_FLAG);
		buf_rdy = ((flag & NEX_BUF_RDY_FLAG) == NEX_BUF_RDY_FLAG);
		lvl_rdy = ((flag & NEX_LVL_RDY_FLAG) == NEX_LVL_RDY_FLAG);
		cnn_rdy = ((flag & AI_OUTPUT_RDY_FLAG) == AI_OUTPUT_RDY_FLAG);

		// a message has been received via one of the UART-lines (async)
		if(inc_msg)
		{
			c = uart_mbox.pop(false);
			uint8_t stat = pCore->handle(c);
			if(stat != 0)
			{
				e_handler.act(delivery_fail, mailbox_e);
			}
		}

		// the bins of the RTA are ready to be displayed (sync)
		if(buf_rdy && (core_fsm.cur_state == s_rta))
		{
			for(uint8_t i = 0; i < N_BANDS; i++)
			{
				nextion.tx(nextion.bin_id_buf[i], (void*)&nextion.disp_data[i], u8_num_val);
			}
		}

		// the SPL level is ready to be displayed
		if(lvl_rdy && (core_fsm.cur_state == s_rta))
		{
			nextion.tx(NEX_TX_SPL_FIELD, (void*)&nextion.disp_lvl, i16_num_val);
			if(core_fsm.threshold_pcnt < nextion.disp_lvl)
			{
				gpio_D3.set(true);
			}
			else
			{
				gpio_D3.set(false);
			}

		}
		if(lvl_rdy && (core_fsm.cur_state == s_ai))
		{
			nextion.tx(NEX_TX_THRESHOLD_DISPLAY, (void*)&nextion.disp_lvl, u8_num_val);
			if(core_fsm.threshold_pcnt < nextion.disp_lvl)
			{
				uint8_t p = 100;
				gpio_D3.set(true);
				nextion.tx(NEX_TX_THRESHOLD_PEAK, (void*)&p, u8_num_val);
			}
			else
			{
				uint8_t p = 0;
				gpio_D3.set(false);
				nextion.tx(NEX_TX_THRESHOLD_PEAK, (void*)&p, u8_num_val);
			}
		}

		// inference has been run and classification data can be displayed
		if(cnn_rdy && (core_fsm.cur_state == s_ai))
		{
			nextion.tx(NEX_TX_CLASS_NAME_FIELD_1, (void*)class_map[cnn_instance.top3_class_idx[0]], txt_val);
			nextion.tx(NEX_TX_CLASS_SCORE_FIELD_1, (void*)&cnn_instance.top3_class_score[0], f32_num_val);

			nextion.tx(NEX_TX_CLASS_NAME_FIELD_2, (void*)class_map[cnn_instance.top3_class_idx[1]], txt_val);
			nextion.tx(NEX_TX_CLASS_SCORE_FIELD_2, (void*)&cnn_instance.top3_class_score[1], f32_num_val);

			nextion.tx(NEX_TX_CLASS_NAME_FIELD_3, (void*)class_map[cnn_instance.top3_class_idx[2]], txt_val);
			nextion.tx(NEX_TX_CLASS_SCORE_FIELD_3, (void*)&cnn_instance.top3_class_score[2], f32_num_val);

			nextion.tx(NEX_TX_ZZZ_IMG, NULL, hide_graphics);
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
		e_handler.act(mem_null, core_e);
	}
}



void core::set_ceq(uint8_t ceq)
{
	ceq_request = ceq;
}



void core::update_ceq(void)
{
	ceq = ceq_request;
}
