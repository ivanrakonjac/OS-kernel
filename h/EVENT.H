#ifndef _EVENT_H
#define _EVENT_H

typedef unsigned char IVTNo;

class KernelEv;

#include "IVTENTRY.H"

#define PREPAREENTRY(number,flag)\
void interrupt intRut_##number(...);\
IVTEntry ivtEntry_##number(number,intRut_##number);\
void interrupt intRut_##number(...){\
	IVTEntry::signal(number);\
	if(flag) IVTEntry::useOld(number);\
}\

class Event {

public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait();

protected:
	friend class KernelEv;
	void signal();// can call KernelEv

private:
	KernelEv* myImpl;
};

#endif
