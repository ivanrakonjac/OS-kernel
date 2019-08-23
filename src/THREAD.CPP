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
	return System::allThreads->getPCBwithID(id);
}

Thread::Thread(StackSize stackSize,Time timeSlice){

}

Thread::~Thread(){

}
