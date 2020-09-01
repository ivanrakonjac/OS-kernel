#ifndef _EVENT_H
#define _EVENT_H
#include "IVTENTRY.H"
typedef unsigned char IVTNo;

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
