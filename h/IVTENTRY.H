#ifndef _IVT_ENTRY_H
#define _IVT_ENTRY_H

#include "Event.h"

class IVTEntry{
	
public:
	IVTEntry(IVTNo number, void interrupt(*newEvent)(...));
	~IVTEntry();

	static void signal(IVTNo number);
	static void useOld(IVTNo number);

	KernelEv* getEvent(){
		return this->kernEvent;
	}
	void setEvent(KernelEv* kev){
		this->kernEvent = kev;
	}

private:
	void interrupt (*oldInterrupt)(...);
	KernelEv* kernEvent;
	IVTNo ivtNumber;
};

#endif
