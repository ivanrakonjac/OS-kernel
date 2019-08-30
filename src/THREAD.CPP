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
	myPCB->waitToComplete();

	lock
	delete myPCB;
	unlock
}

void dispatch(){
	System::dispatch();
}

void Thread::blockSignalGlobally(SignalId id){
	System::globallyBlockedSignals[id]=1;
}
void Thread::unblockSignalGlobally(SignalId id){
	System::globallyBlockedSignals[id]=0;
}

void Thread::signal(SignalId id){
	lock
	myPCB->signal(id);
	unlock
}

void Thread::registerHandler(SignalId id,SignalHandler sh){
	lock
	myPCB->registerHandler(id,sh);
	unlock
}

void Thread::unregisterAllHandlers(SignalId id){
	lock
	myPCB->unregisterAllHandlers(id);
	unlock
}

void Thread::swap(SignalId id, SignalHandler sh1,SignalHandler sh2){
	lock
	myPCB->swap(id,sh1,sh2);
	unlock
}

void Thread::blockSignal(SignalId id){
	lock
	myPCB->blockSignal(id);
	unlock
}

void Thread::unblockSignal(SignalId id){
	lock
	myPCB->unblockSignal(id);
	unlock
}
