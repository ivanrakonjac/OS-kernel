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
	//cout<<"Brisem event: "<<this<<endl;
	delete myImpl;
	unlock
}

void Event::wait(){
	//cout<<"Event::wait()"<<endl;
	myImpl->wait();
}

void Event::signal(){
	//cout<<"Event::signal()"<<endl;
	myImpl->signal();
}
