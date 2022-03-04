#include <mailbox.h>

// global instances
mbox uart_mbox;
mbox i2s_mbox;



mbox::mbox(void)
{

}



void mbox::init(void)
{
	hQueue = osMessageQueueNew(MBOX_SIZE, sizeof(cmd), NULL);

	if(IS_NULL(hQueue))
	{
		err e(mem_null, mailbox_e, HIGH, NULL);
		e.handle();
	}
}



void mbox::push(cmd cmd, uint8_t timeout)
{
	osStatus_t stat;

	stat = osMessageQueuePut(hQueue, (void*)&cmd, cmd.prio, timeout);
	if(stat != osOK)
	{
		err e(delivery_fail, mailbox_e, LOW, &stat);
		e.handle();
	}
}



cmd mbox::pop(uint8_t timeout)
{
	osStatus_t stat;
	cmd c;

	stat = osMessageQueueGet(hQueue, &c, NULL, timeout);
	if(stat != osOK)
	{
		err e(delivery_fail, mailbox_e, LOW, &stat);
		e.handle();
	}
	return c;
}



uint8_t mbox::data_avail(void)
{
	return osMessageQueueGetCount(hQueue);
}



void mbox::reset(void)
{
	osMessageQueueReset(hQueue);
}
