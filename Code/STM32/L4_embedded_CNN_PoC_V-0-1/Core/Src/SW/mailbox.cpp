#include <mailbox.h>

// global instances
mbox uart_mbox;



mbox::mbox(void)
{

}



void mbox::init(void)
{
	hQueue = osMessageQueueNew(QUEUE_SIZE, sizeof(cmd), NULL);

	if(IS_NULL(hQueue))
	{
		err e(mem_null, mailbox_e, HIGH);
		e.handle();
	}
}



void mbox::push(cmd cmd, bool from_ISR)
{
	osStatus_t stat;
	if(from_ISR)
	{
		stat = osMessageQueuePut(hQueue, (void*)&cmd, cmd.prio, QUEUE_FROM_ISR);
	}
	else
	{
		stat = osMessageQueuePut(hQueue, (void*)&cmd, cmd.prio, QUEUE_TIMEOUT);
	}

	if(stat != osOK)
	{
		err e(generic, mailbox_e, LOW);
		e.handle();
	}
}



cmd mbox::pop(bool from_ISR)
{
	osStatus_t stat;
	cmd c;
	if(from_ISR)
	{
		stat = osMessageQueueGet(hQueue, &c, NULL, QUEUE_FROM_ISR);
	}
	else
	{
		stat = osMessageQueueGet(hQueue, &c, NULL, QUEUE_NO_TIMEOUT);
	}

	if(stat != osOK)
	{
		err e(generic, mailbox_e, LOW);
		e.handle();
	}
	return c;
}



bool mbox::data_avail(void)
{
	if(osMessageQueueGetCount(hQueue) != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
