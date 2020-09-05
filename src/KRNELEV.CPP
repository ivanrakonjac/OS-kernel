#include "KRNELEV.H"
#include "PCB.H"
#include "EVENT.H"
#include "System.H"
#include "Schedule.h"
#include "IVTENTRY.H"

int KernelEv::kerEvNum=0;

KernelEv::KernelEv(unsigned ivtNo){

	this->value = 1;
	this->entryNum=ivtNo;

	this->blokiranaNit=0;
	this->owner = PCB::runnig;

	System::interruptEntries[ivtNo]->kernEvent=this;
	KernelEv::kerEvNum++;

}

KernelEv::~KernelEv(){
	lock
	System::interruptEntries[this->entryNum]->kernEvent=0;
	KernelEv::kerEvNum--;
	unlock

};

void KernelEv::wait(){
	lock

	if(this->owner == PCB::runnig){
		this->owner->setState(PCB::BLOCKED);
		unlock
		System::dispatch();
	}else{
		unlock
	}

	return;
}

void KernelEv::signal(){
	lock

	if(this->owner->getState()==PCB::BLOCKED){
		this->owner->setState(PCB::READY);
		Scheduler::put(this->owner);
	}

	unlock
	return;
}
