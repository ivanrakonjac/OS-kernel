#include "THREAD.H"
#include "PCB.H"
#include "SYSTEM.H"
#include "PCBLIST.H"


void Thread::start(){
	this->myPCB->start();
}

void Thread::waitToComplete(){
	this->myPCB->waitToComplete();

}

ID Thread::getId(){
	return this->myPCB->getID();
}

ID Thread::getRunningId(){
	PCB::getRunningId();
}

Thread* Thread::getThreadById(int id){
	return System::allThreads->getPCBwithID(id)->getThread();
}


Thread::Thread(StackSize ss,Time ts){
	myPCB = new PCB(ss,ts,this);
}

Thread::~Thread(){
	lock
	cout<<"NIT: "<<getId()<<" Thread::~Thread()"<<endl;
	unlock
	myPCB->waitToComplete();

	lock
	delete myPCB;
	unlock
}

void dispatch(){
	System::dispatch();
}
