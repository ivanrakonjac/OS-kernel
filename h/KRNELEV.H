#ifndef _KRNEL_EV_H
#define _KRNEL_EV_H

#include "Event.h"

class PCB;

class KernelEv{
	
public:

	static int kerEvNum;

	KernelEv(unsigned ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:
	int value;
	unsigned entryNum;

	PCB* owner;
	PCB* blokiranaNit;
};

#endif
