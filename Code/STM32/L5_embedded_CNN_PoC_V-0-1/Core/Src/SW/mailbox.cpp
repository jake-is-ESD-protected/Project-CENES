#include <mailbox.h>

mbox::mbox(osThreadId_t receiving_thread)
{
	queue_id = osMessageCreate(osMessageQ(queue), receiving_thread);
}



void mbox::push(cmd cmd, bool from_ISR)
{
	osEvent ev;
	if(from_ISR)
	{
		ev.status = osMessagePut(queue_id, *((uint32_t*)(&cmd)), QUEUE_FROM_ISR);
	}
	else
	{
		ev.status = osMessagePut(queue_id, *((uint32_t*)(&cmd)), QUEUE_TIMEOUT);
	}

	if(ev.status != osOK)
	{
		err e(generic, SW, LOW);
		e.handle();
	}
}



cmd mbox::pop()
{
	osEvent ev;
	ev = osMessageGet(queue_id, QUEUE_TIMEOUT);
	if(ev.status == osEventTimeout)
	{
		cmd c = {.type = no_info, .origin = MAIN, .prio = LOW, .unused = 0};
		return c;
	}
	else if(ev.status == osEventMessage)
	{
		return *((cmd*)(&(ev.value)));
	}
	else
	{
		cmd c = {.type = no_info, .origin = MAIN, .prio = LOW, .unused = 0};
		err e(generic, SW, LOW);
		e.handle();
		return c;
	}
}
