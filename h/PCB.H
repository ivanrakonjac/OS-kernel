#ifndef _PCB_H_
#define _PCB_H_

#include <iostream.h>
#include <dos.h>

class Thread;
class PCB_LIST;

class PCB{

public:

	static int ajdi;
	static PCB* runnig;

	enum State { READY, BLOCKED, FINISHED, NEW };

	PCB();
	PCB(unsigned long stackSize, unsigned int timeSlice, Thread* myTr);
	~PCB();

	void start();
	void waitToComplete();

	int getID() const{
		return this->id;
	}
	int getQuantum() const {
		return this->quantum;
	}
	State getState() const {
		return this->state;
	}
	Thread* getThread() const{
		return this->myThread;
	}

	void setID(int id){
		this->id=id;
	}
	void setState(State s){
		this->state = s;
	}

	static int getRunningId(){
		return runnig->id;
	}

	static void wrapper();

private:

	friend void interrupt timer(...);

	unsigned bp;
	unsigned sp;
	unsigned ss;
	State state;
	unsigned* stack;
	int quantum;
	int id;
	Thread* myThread;
	PCB_LIST* blockedOnThread;



};

#endif
