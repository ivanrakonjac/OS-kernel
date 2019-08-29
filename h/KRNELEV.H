#ifndef _KRNEL_EV_H
#define _KRNEL_EV_H

#include "Event.h"

class PCB;

class KernelEv{
	
public:
	KernelEv(IVTNo ivtNo, Event* e);
	void wait();
	void signal();

private:
	int value;
	IVTNo ivtKeNumber;
	Event* event;
	PCB* owner;
};

#endif
