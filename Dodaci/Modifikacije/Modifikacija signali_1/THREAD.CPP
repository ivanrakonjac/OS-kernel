#include "THREAD.H"
#include "PCB.H"
#include "SYSTEM.H"
#include "PCBLIST.H"

int Thread::threadNum=0;


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
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(int id){

	if(id==-1){
		PCB::main->setThread(new Thread());
		PCB::main->getThread()->setMyPCB(PCB::main);
		return PCB::main->getThread();
	}else{
		return System::allThreads->getPCBwithID(id)->getThread();
	}
}


Thread::Thread(StackSize ss,Time ts){
	myPCB = new PCB(ss,ts,this);
	lock
	Thread::threadNum=Thread::threadNum + 1;
	unlock
}

Thread::~Thread(){
	myPCB->waitToComplete();

	lock
	delete myPCB;
	Thread::threadNum=Thread::threadNum - 1;
	unlock
}

void dispatch(){
	System::dispatch();
}

void Thread::blockSignalGlobally(SignalId id){
	System::globallyBlockedSignals[id]=1;
	cout<<"System::globallyBlockedSignals["<<id<<"]"<<System::globallyBlockedSignals[id]<<endl;
}
void Thread::unblockSignalGlobally(SignalId id){
	System::globallyBlockedSignals[id]=0;
}

void Thread::signal(SignalId id/*SignalHandler sh*/){

	myPCB->signal(id);

}

void Thread::registerHandler(SignalId id,SignalHandler sh[],int maska){
	lock
	myPCB->registerHandler(id,sh,maska);
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

