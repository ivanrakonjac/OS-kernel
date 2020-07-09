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
	//cout<<"Brisem kernEv "<<this->entryNum<<" | owner-state:"<<this->owner->getState()<<endl;
	if(this->owner->getState()==PCB::BLOCKED){
		this->owner->setState(PCB::READY);
		cout<<"Brisem kernEv"<<this->owner->getState()<<endl;
		//Scheduler::put(owner);
	}
	System::interruptEntries[this->entryNum]->kernEvent=0;
	KernelEv::kerEvNum--;
	unlock

};

void KernelEv::wait(){
	lock

	//cout<<"KernelEvent::wait()"<<endl;

	/*if(PCB::runnig == this->owner){
		if(this->value==0){
			this->blokiranaNit=PCB::runnig;
			PCB::runnig->setState(PCB::BLOCKED);
			unlock
			System::dispatch();
			return;
		}
		else{
			if(this->value==1) value=0;
		}
	}*/

	if(this->owner == PCB::runnig){
		//cout<<"KernelEvent::wait()"<<endl;
		this->owner->setState(PCB::BLOCKED);
		//cout<<"Zabodena "<<owner->getID()<<endl;
		unlock
		System::dispatch();
	}else{
		unlock
	}

	//unlock
	return;
}

void KernelEv::signal(){
	lock
	//cout<<"KernelEvent::signal()"<<endl;
	System::tstBrojac++;

	/*if(blokiranaNit == 0){
		value = 1;
	}else{

		blokiranaNit->setState(PCB::READY);
		Scheduler::put(blokiranaNit);
		blokiranaNit=0;

		unlock
		return;
	}*/

	this->owner->setState(PCB::READY);
	Scheduler::put(this->owner);

	unlock
	return;
}
