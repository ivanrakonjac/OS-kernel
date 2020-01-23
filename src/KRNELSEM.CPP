#include "KRNELSEM.H"
#include "TIMEBLCK.H"
#include "PCBLIST.H"
#include "SYSTEM.H"
#include "PCB.H"
#include "SCHEDULE.H"

KernelSem::KernelSem(int inic){
	asm cli;
	value = inic;
	blockList = new PCB_LIST();
	asm sti;
}

KernelSem::~KernelSem(){
	asm cli;
	delete blockList;
	asm sti;
}

int KernelSem::wait(int maxTimeToWait){

	asm cli;

	PCB::runnig->setState(PCB::BLOCKED);

	if(maxTimeToWait == 0){
		value = value - 1;

		if(value>=0){
			asm sti;
			return 1;
		}

		blockList->insert(PCB::runnig);
	}
	else{
		System::timeBlockList->insert(PCB::runnig,maxTimeToWait,this);
	}

	asm sti;

	System::dispatch();

	/*if(!PCB::runnig->getSignalWoken()){
		PCB::runnig->setSignalWoken(0);
		return 0;
	}*/

	return 0;
}

int KernelSem::signal(int n){
	asm cli;

	if(n<0){
		asm sti;
		return n;
	}
	else if(n == 0 ){
		value = value + 1;

		PCB* temp = blockList->popFront();
		if(temp == 0){
			asm sti;
			return n;
		}

		temp->setState(PCB::READY);
		//temp->setSignalWoken(1);

		if(temp!=PCB::main) Scheduler::put(temp);

		asm sti;
		return 0;
	}
	else if(n>0){
		int cntr = 0;
		value = value + n;

		for (int i= 0; i < n; ++i) {
			PCB* temp = blockList->popFront();
			if(temp == 0){
				asm sti;
				return cntr;
			}

			temp->setState(PCB::READY);
			//temp->setSignalWoken(1);

			if(temp!=PCB::main) Scheduler::put(temp);
		}
		asm sti;
		return cntr;
	}
}

int KernelSem::val() const{
	return this->value;
}

void KernelSem::incVal(){
	value = value + 1;
}
