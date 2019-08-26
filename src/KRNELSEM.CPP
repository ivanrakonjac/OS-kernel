#include "KRNELSEM.H"
#include "TIMEBLCK.H"
#include "PCBLIST.H"
#include "SYSTEM.H"
#include "PCB.H"
#include "SCHEDULE.H"

KernelSem::KernelSem(int inic){
	value = inic;
	asm cli
	cout<<"KERNELSEM(): val = "<<value<<endl;
	asm sti
	lock
	blockList = new PCB_LIST();
	unlock
}

KernelSem::~KernelSem(){
	lock
	delete blockList;
	unlock
}

int KernelSem::wait(int maxTimeToWait){
	value = value - 1;

	asm cli;

	cout<<"WAIT(): val = "<<value<<endl;

	if(value>0){
		asm sti
		return 1;
	}

	PCB::runnig->setState(PCB::BLOCKED);

	if(maxTimeToWait == 0){
		blockList->insert(PCB::runnig);
		cout<<"USPAVANA blockList NIT: "<<PCB::runnig->getID()<<endl;
	}
	else{
		System::timeBlockList->insert(PCB::runnig,maxTimeToWait,this);
		cout<<"USPAVANA timeBlockList NIT: "<<PCB::runnig->getID()<<endl;
	}

	asm sti

	System::dispatch();

	if(PCB::runnig->getSignalWoken()){
		PCB::runnig->setSignalWoken(0);
		return 0;
	}

	return 1;
}

int KernelSem::signal(int n){
	asm cli

	if(n<0) return n;
	else if(n == 0 ){
		value = value + 1;

		cout<<"SIGNAL(0): val = "<<value<<endl;

		PCB* temp = blockList->popFront();
		if(temp == 0) return n;

		temp->setState(PCB::READY);
		temp->setSignalWoken(1);

		cout<<"PROBUDJENA NIT: "<<temp->getID()<<endl;

		Scheduler::put(temp);

		return 0;
	}
	else if(n>0){
		int cntr = 0;
		value = value + n;

		cout<<"SIGNAL(n): val = "<<value<<endl;

		for (int i= 0; i < n; ++i) {
			PCB* temp = blockList->popFront();
			if(temp == 0) return cntr;

			temp->setState(PCB::READY);
			temp->setSignalWoken(1);

			cout<<"PROBUDJENA NIT: "<<temp->getID()<<endl;

			Scheduler::put(temp);
		}
		asm sti;
		return cntr;
	}
	asm sti

}

int KernelSem::val() const{
	return this->value;
}

void KernelSem::incVal(){
	value = value + 1;
}