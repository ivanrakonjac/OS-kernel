#include "KRNELEV.H"
#include "PCB.H"
#include "EVENT.H"
#include "System.H"
#include "Schedule.h"

KernelEv::KernelEv(IVTNo ivtNo, Event* e){
	this->ivtKeNumber = ivtNo;
	this->event = e;
	this->owner = PCB::runnig;
	this->value = 1;
	this->blokiranaNit=0;
}

void KernelEv::wait(){
	lock
	if(PCB::runnig == owner){
		if(this->value==0){
			this->blokiranaNit=(PCB*)PCB::runnig;
			PCB::runnig->setState(PCB::BLOCKED);
			unlock
			System::dispatch();
			return;
		}
		else{
			if(this->value==1) value=0;
		}
	}
	else{
		unlock
		return;
	}
}

void KernelEv::signal(){
	lock
	System::tstBrojac++;
	if(blokiranaNit == 0) value = 1;
	else{
		owner->setState(PCB::READY);
		if(owner!=PCB::main) Scheduler::put(owner);
		unlock
		return;
	}
	unlock
}

KernelEv::~KernelEv(){
	System::interruptEntries[this->ivtKeNumber]->setEvent(0);
}
