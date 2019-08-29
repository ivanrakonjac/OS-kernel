#include "KRNELEV.H"
#include "PCB.H"
//#include "EVENT.H"
#include "System.H"
#include "Scheduler.h"

KernelEv::KernelEv(IVTNo ivtNo, Event* e){
	this->ivtKeNumber = ivtNo;
	this->event = e;
	this->owner = PCB::runnig;
	this->value = 0;
	cout<<"OWNER: "<<PCB::runnig->getID()<<endl;
}

void KernelEv::wait(){
	if(PCB::runnig == owner){
		cout<<"KernelEv::wait()"<<endl;
		asm cli
		//value = value - 1;
		//cout<<"value = "<<value<<endl;
		//if(value < 0){
			PCB::runnig->setState(PCB::BLOCKED);
			System::dispatch();
			asm sti
			return;
		//}
		asm sti
		return;
	}
	else return;
}

void KernelEv::signal(){
	cout<<"KernelEv::signal()"<<endl;
	System::tstBrojac++;
	asm cli
	//if(value<0){
		owner->setState(PCB::READY);
		Scheduler::put(owner);
		//value = 0;
		System::dispatch();
	//}
	asm sti;
	return;
}
