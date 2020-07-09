#ifndef _IVT_ENTRY_H
#define _IVT_ENTRY_H

#include "Event.h"

typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(numEntry,callOld)\
	void interrupt inter##numEntry(...);\
	IVTEntry newEntry##numEntry(numEntry,inter##numEntry);\
	void interrupt inter##numEntry(...){\
	newEntry##numEntry.signal();\
	if (callOld==1) newEntry##numEntry.callOldRoutine();\
}

class IVTEntry{
	
public:
	IVTEntry(unsigned numEntry, pInterrupt newRout);
	~IVTEntry();

	void signal();
	void callOldRoutine();

	/*KernelEv* getEvent(){
		return this->kernEvent;
	}
	void setEvent(KernelEv* kev){
		this->kernEvent = kev;
	}*/

private:
	friend class KernelEv;
	pInterrupt oldRout;
	KernelEv* kernEvent;
	unsigned numEntry;
};

#endif
