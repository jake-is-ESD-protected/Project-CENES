#include "nextion.h"
#include "core.h"

nextionHMI nextion(&huart2);


void nextionHMI::reset(void)
{
	HAL_UART_DeInit(pUart);
	HAL_UART_Init(pUart);
	HAL_StatusTypeDef stat = HAL_UART_Receive_IT(pUart, (uint8_t*)rx_buf, NEX_RX_BUF_SIZE);
	if(stat != HAL_OK)
	{
		e_handler.act(init_fail, nextion_e);
	}
}



void nextionHMI::tx(char* id, void* pVal, nex_tx_t type)
{
	uint8_t len = format(id, pVal, type);
	HAL_UART_Transmit(&huart2, (uint8_t*)tx_buf, len, -1);
}



void nextionHMI::handle(cmd c)
{
	osKernelLock();

	int8_t val = -1;

	switch(c.type)
	{
	case switch_rta:
		val = NEX_PAGE0;
		core_fsm.cur_state = core_fsm.s_rta;
		tx(NULL, (void*)&val, u8_page);
		update_page(val);
		break;

	case switch_ai:
		val = NEX_PAGE1;
		core_fsm.cur_state = core_fsm.s_ai;
		tx(NULL, (void*)&val, u8_page);
		update_page(val);
		break;

	case switch_settings:
		val = NEX_PAGE2;
		core_fsm.cur_state = core_fsm.s_settings;
		tx(NULL, (void*)&val, u8_page);
		update_page(val);
		break;

	case set_A_weight:
		core_fsm.weighting = A_WEIGHTING;
		tx(NEX_TX_SPL_TYPE_FIELD, (char*)"LAF", txt_val);
		break;

	case set_Z_weight:
		core_fsm.weighting = Z_WEIGHTING;
		tx(NEX_TX_SPL_TYPE_FIELD, (char*)"LZF", txt_val);
		break;

	case set_CEQ1:
		core_fsm.set_ceq(CEQ_1S);
		tx(NEX_CEQ_DISPLAY, (char*)"CEQ1''", txt_val);
		break;

	case set_CEQ3:
		core_fsm.set_ceq(CEQ_3S);
		tx(NEX_CEQ_DISPLAY, (char*)"CEQ3''", txt_val);
		break;

	case set_CEQ5:
		core_fsm.set_ceq(CEQ_5S);
		tx(NEX_CEQ_DISPLAY, (char*)"CEQ5''", txt_val);
		break;

	case set_thresh_req:
		tx(NEX_TX_THRESHOLD_GET, NULL, get_slider);
		break;

	case set_thresh_num:
		core_fsm.threshold_pcnt = *((uint8_t*)(c.params));
		break;

	case reset_classes:
		tx(NEX_TX_ZZZ_IMG, NULL, show_graphics);
		break;



	// TODO: expand commands (if needed)
	default:
		break;
	}



	osKernelUnlock();


	// TODO: This should not be needed
	reset();

}



void nextionHMI::update_page(int8_t page)
{
	switch(page)
	{
	case NEX_PAGE0:
	{
		if(core_fsm.weighting == A_WEIGHTING)
		{
			tx(NEX_TX_SPL_TYPE_FIELD, (char*)"LAF", txt_val);
		}
		else
		{
			tx(NEX_TX_SPL_TYPE_FIELD, (char*)"LZF", txt_val);
		}
	} break;

	case NEX_PAGE1:
	{
		if(core_fsm.ceq == CEQ_1S)
		{
			tx(NEX_CEQ_DISPLAY, (char*)"CEQ1''", txt_val);
		}
		else if(core_fsm.ceq == CEQ_3S)
		{
			tx(NEX_CEQ_DISPLAY, (char*)"CEQ3''", txt_val);
		}
		else if(core_fsm.ceq == CEQ_5S)
		{
			tx(NEX_CEQ_DISPLAY, (char*)"CEQ5''", txt_val);
		}
		tx(NEX_TX_THRESHOLD_GET, (void*)&core_fsm.threshold_pcnt, u8_num_val);
	} break;

	case NEX_PAGE2:
	{

	} break;

	default:
	{

	}break;
	}
}



void nextionHMI::rx_callback(void)
{
	cmd c = str2cmd();
	uart_mbox.push(c, MBOX_FROM_ISR, core_fsm.hUI_task, INC_MSG_FLAG);
}
