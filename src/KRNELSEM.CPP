#include "KRNELSEM.H"
#include "TIMEBLCK.H"
#include "PCBLIST.H"
#include "SYSTEM.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "SEM_LIST.H"
#include "queue.h"

KernelSem::KernelSem(int inic){
	asm cli;
	value = inic;
	//blockList = new PCB_LIST();
	//timeBlockList=new TIME_BLCK_LIST();
	blockList = new Queue();
	timeBlockList = new Queue();
	System::allSem->insert(this);
	asm sti;
}

KernelSem::~KernelSem(){
	asm cli;
	delete blockList;
	delete timeBlockList;
	System::allSem->remove(this);
	asm sti;
}

int KernelSem::wait(int maxTimeToWait){
	PCB::runnig->setTimeUnblock(0);

	if (--value < 0) {
		PCB::runnig->setState(PCB::BLOCKED);

		if(maxTimeToWait == 0){
			//blockList->insert(PCB::runnig);
			blockList->put(PCB::runnig);
		}
		else{
			//this->timeBlockList->insert(PCB::runnig,maxTimeToWait,this);
			timeBlockList->put(PCB::runnig);
			PCB::runnig->timeToSleep = maxTimeToWait;
		}
		System::dispatch();
	}

	if(PCB::runnig->getTimeUnblock()){
		//ponistavam bit vremenskog budjenja, da bi ga koristio i sl put
		PCB::runnig->setTimeUnblock(0);
		return 0;
	}
	else{
		return 1;
	}

}

int KernelSem::signal(int n){

	lock

	if(n<0){
		unlock
		return n;
	}
	else if(n == 0 ){
		value = value + 1;

		PCB* temp = blockList->get();//popFront();

		if(temp==0){
			//TIME_BLCK_LIST::timeNode* tndp=timeBlockList->popFront();
			temp = timeBlockList->get();
			if(temp==0){
				unlock
				return 0;
			}
			//temp=tndp->data;
		}

		if(temp == 0){
			//cout<<"Nema nikoga"<<endl;
			unlock
			return n;
		}

		temp->setState(PCB::READY);

		Scheduler::put(temp);

		unlock
		return 0;
	}
	else if(n>0){
		int cntr = 0;
		value = value + n;

		for (int i= 0; i < n; ++i) {
			//PCB* temp = blockList->popFront();
			PCB* temp = blockList->get();
			if(temp == 0){
				break;
			}

			cntr++;
			temp->setState(PCB::READY);

			Scheduler::put(temp);
		}


		for (int j= cntr; j < n; ++j) {

			//this->timeBlockList->stampaj();

			//TIME_BLCK_LIST::timeNode* temp2=timeBlockList->popFront();

			//cout<<temp2<<endl;

			/*if(temp2 == 0){
				unlock
				return cntr;
			}*/

			PCB* temp = timeBlockList->get();
			if(temp == 0){
				unlock
				return cntr;
			}

			cntr++;
			temp->setState(PCB::READY);
			//temp->setSignalWoken(1);

			Scheduler::put(temp);
		}


		unlock
		return cntr;
	}
}

int KernelSem::val() const{
	return this->value;
}

void KernelSem::incVal(){
	value = value + 1;
}

void KernelSem::update() {
	Elem *t = timeBlockList->first;
	Elem *p = 0, *d = 0;
	while (t) {
		t->pcb->timeToSleep--;
		if (t->pcb->timeToSleep > 0) {
			p = t;
			t = t->next;
		} else {
			t->pcb->state = PCB::READY;
			t->pcb->setTimeUnblock(1);
			Scheduler::put(t->pcb);
			d = t;
			t = t->next;
			if (!p)
				timeBlockList->first = t;
			else
				p->next = t;
			delete d;
			value++;
		}
	}
}
