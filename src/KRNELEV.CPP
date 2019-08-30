#include "KRNELEV.H"
#include "PCB.H"
//#include "EVENT.H"
#include "System.H"
#include "Schedule.h"

KernelEv::KernelEv(IVTNo ivtNo, Event* e){
	this->ivtKeNumber = ivtNo;
	this->event = e;
	this->owner = PCB::runnig;
	this->value = 0;
}

void KernelEv::wait(){
	if(PCB::runnig == owner){
		asm cli
		PCB::runnig->setState(PCB::BLOCKED);
		System::dispatch();
		asm sti
		return;
		asm sti
		return;
	}
	else return;
}

void KernelEv::signal(){
	System::tstBrojac++;
	asm cli
	owner->setState(PCB::READY);
	Scheduler::put(owner);
	System::dispatch();
	asm sti;
	return;
}
