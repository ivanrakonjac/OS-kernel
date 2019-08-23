#include "THREAD.H"
#include "PCB.H"


void Thread::start(){
	this->myPCB->start();
}

void Thread::waitToComplete(){
	this->myPCB->waitToComplete();
}

ID Thread::getId(){
	this->myPCB->getID();
}

ID Thread::getRunningId(){
	PCB::getRunningId();
}

Thread* Thread::getThreadById(int id){
}

Thread::Thread(){
	myPCB = new PCB();
}

Thread::Thread(StackSize ss,Time ts){
	myPCB = new PCB(ss,ts,this);
}

Thread::~Thread(){

}
