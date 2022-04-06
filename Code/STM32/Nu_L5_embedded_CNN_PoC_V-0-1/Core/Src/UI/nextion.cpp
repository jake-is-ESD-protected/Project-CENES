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
		// TODO: err-handler
	}
}



void nextionHMI::tx(char* id, void* pVal, nex_tx_t type)
{
	uint8_t len = format(id, pVal, type);
	HAL_UART_Transmit(&huart2, (uint8_t*)tx_buf, len, -1);
}



void nextionHMI::handle(cmd c)
{

	int8_t val = -1;

	switch(c.type)
	{
	case switch_rta:
		val = NEX_PAGE0;
		core_fsm.cur_state = core_fsm.s_rta;
		break;
	case switch_ai:
		val = NEX_PAGE1;
		core_fsm.cur_state = core_fsm.s_ai;
		break;
	case switch_settings:
		val = NEX_PAGE2;
		core_fsm.cur_state = core_fsm.s_settings;
		break;
	default:
		break;
		// TODO: default statement
	}

	osKernelLock();
	tx(NULL, (void*)&val, u8_page);
	osKernelUnlock();


	// TODO: This should not be needed
	reset();

}



void nextionHMI::rx_callback(void)
{
	cmd c = str2cmd();
	uart_mbox.push(c, MBOX_FROM_ISR, core_fsm.hUI_task, INC_MSG_FLAG);
	osThreadFlagsSet(core_fsm.hUI_task, INC_MSG_FLAG);
}
