#include "EVENT.H"
#include "SYSTEM.H"
#include "KRNELEV.H"
#include "IVTENTRY.H"

Event::Event(IVTNo n){
	lock
	myImpl  = new KernelEv(n);
	unlock
}

Event::~Event(){
	lock
	delete myImpl;
	unlock
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}
