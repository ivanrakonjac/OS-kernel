#ifndef _PCB_H_
#define _PCB_H_

#include <iostream.h>
#include <dos.h>
#include "Thread.h"
#include "system.h"
#include "EXEWAIT.H"

class Thread;
class PCB_LIST;
class SignalHandlerList;
class ExecutionWaitingList;

class PCB{
	friend class KernelSem;
public:

	static int ajdi;
	static PCB* runnig;
	static PCB* main;

	enum State { READY, BLOCKED, FINISHED, NEW };

	static int pcbNum;

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
	int getTimeUnblock(){
		return this->timeUnblock;
	}
	Thread* getThread(){
		return this->myThread;
	}


	void setID(int id){
		this->id=id;
	}
	void setState(State s){
		this->state = s;
	}
	void setTimeUnblock(int val){
		this->timeUnblock=val;
	}
	void setThread(Thread* thread){
		this->myThread=thread;
	}


/*	int getSignalWoken() const{
		return this->signalWoken;
	}
	void setSignalWoken(int i){
		this->signalWoken = i;
	}
*/


	static int getRunningId(){
		//cout<<"PCB::running "<<PCB::runnig->id<<endl;
		return PCB::runnig->id;
	}

	static void wrapper();


	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);


	PCB_LIST* getListu(){
		return this->blockedOnThread;
	}

protected:
	void systemSignal(SignalId id);

private:

	friend void interrupt timer(...);
	friend class ExecutionWaitingList;

	Time timeToSleep;
	unsigned bp;
	unsigned sp;
	unsigned ss;
	State state;
	unsigned* stack;
	int quantum;
	int id;
	int killedBySignal;
	Thread* myThread;
	PCB_LIST* blockedOnThread;
	int timeUnblock;

	PCB* owner;
	SignalHandlerList* signalArray[16]; //svaki od signala ima pokazivac na listu handlera
	int signalBlockArray[16];
	ExecutionWaitingList* signalsWaitngForExecution;

};

#endif
