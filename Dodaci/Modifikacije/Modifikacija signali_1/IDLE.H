#ifndef _IDLE_H_
#define _IDLE_H_

#include "THREAD.H"

class Idle : public Thread {
public:

	Idle();

protected:
	void run();
};

#endif
