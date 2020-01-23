#include "EVENT.H"
#include "SYSTEM.H"
#include "KRNELEV.H"
#include "IVTENTRY.H"

Event::Event(IVTNo n){
	lock
	myImpl  = new KernelEv(n,this);
	if(System::interruptEntries[n] !=0 ) System::interruptEntries[n]->setEvent(myImpl);
	unlock
}

Event::~Event(){
	asm cli;
	delete myImpl;
	asm sti;
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}
