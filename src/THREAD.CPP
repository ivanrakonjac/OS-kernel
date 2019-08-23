#include "THREAD.H"
#include "PCB.H"
#include "SYSTEM.H"
#include "PCBLIST.H"


void Thread::start(){
	this->myPCB->start();
}

void Thread::waitToComplete(){
	cout<<"Nit "<<this->getId()<<" waitToComplete()"<<endl;
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

Thread::Thread(){
	myPCB = new PCB();
}

Thread::Thread(StackSize ss,Time ts){
	myPCB = new PCB(ss,ts,this);
}

Thread::~Thread(){
	myPCB->waitToComplete();

	lock
	delete myPCB;
	unlock
}
