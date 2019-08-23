#ifndef _PCB_H_
#define _PCB_H_

#include <iostream.h>
#include <dos.h>
#include "THREAD.H"

class PCB{

public:

	static int ajdi;
	static PCB* runnig;

	enum State { READY, RUNNING, BLOCKED, FINISHED, NEW };

	PCB();
	PCB(unsigned long stackSize, unsigned int timeSlice, void (*body)());
	~PCB();

	void start();
	void waitToComplete();

	int getID() const{
		return this->id;
	}

	static int getRunningId(){
		return runnig->id;
	}

	static void wraper();

private:

	friend void interrupt timer(...);
	friend void exitThread();
	friend void f();

	unsigned bp;
	unsigned sp;
	unsigned ss;
	State state;
	unsigned* stack;
	int quantum;
	int id;
	Thread thread;



};

#endif
