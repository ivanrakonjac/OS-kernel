#include "KRNELEV.H"
#include "PCB.H"
#include "EVENT.H"
#include "System.H"
#include "Schedule.h"

KernelEv::KernelEv(IVTNo ivtNo, Event* e){
	this->ivtKeNumber = ivtNo;
	this->event = e;
	this->owner = PCB::runnig;
	this->value = 0;
}

void KernelEv::wait(){
	asm cli;
	if(PCB::runnig == owner){
		PCB::runnig->setState(PCB::BLOCKED);
		asm sti;
		System::dispatch();
		return;
	}
	else{
		asm sti;
		return;
	}
}

void KernelEv::signal(){
	asm cli;
	System::tstBrojac++;
	owner->setState(PCB::READY);
	if(owner!=PCB::main) Scheduler::put(owner);
	asm sti;
	return;
}

KernelEv::~KernelEv(){
	System::interruptEntries[this->ivtKeNumber]->setEvent(0);
}
