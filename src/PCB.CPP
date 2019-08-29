//PCB.CPP

#include "PCB.H"
#include "SYSTEM.H"
#include "THREAD.H"
#include "PCBLIST.H"
#include "SCHEDULE.H"

int PCB::ajdi=0;
PCB* PCB::runnig= new PCB();
PCB* PCB::main = PCB::runnig;

PCB::PCB(){
	state=PCB::READY;
	quantum = 1;
	id=-1;
	myThread=0;
	blockedOnThread=0;
	signalWoken = 0;

	stack=0;

	sp=0;
	ss=0;
	bp=0;
}

PCB::PCB(unsigned long stackSize, unsigned int timeSlice, Thread* myTr){

		this->state=PCB::NEW;
		this->quantum=timeSlice;
		this->id=++ajdi;
		this->signalWoken = 0;
		this->myThread=myTr;
		this->blockedOnThread = new PCB_LIST();

		asm cli;

		if(stackSize>65536) stackSize = 65536;
		StackSize realStackSize = stackSize/sizeof(unsigned int);

		stack = new unsigned [stackSize/sizeof(unsigned int)];
		stack[realStackSize-1] = 0x200;

		stack[realStackSize-2] = FP_SEG(PCB::wrapper);
		stack[realStackSize-3] = FP_OFF(PCB::wrapper);

		sp = FP_OFF(stack+realStackSize-12);
		ss = FP_SEG(stack+realStackSize-12);
		bp = FP_OFF(stack+realStackSize-12);

		System::allThreads->insert(this);

		asm sti;
}

PCB::~PCB(){
	System::allThreads->remove(this);
	delete blockedOnThread;
	if(this->stack)
		delete [] this->stack;
}

void PCB::start(){
	lock
	if(this->state == PCB::NEW){
		if(this != System::idleThread->myPCB){
			this->state = PCB::READY;
			Scheduler::put((PCB*)this);
			cout<<"ID: "<<this->getID()<<" | state: "<<this->getState()<<endl;
		}
	}
	unlock
}

void PCB::waitToComplete(){
	lock
	if(this->myThread == System::idleThread->myPCB || this==runnig){
		lock
		cout<<"this==running"<<endl;
		unlock
		return;
	}
	if(this->state == FINISHED || this->state == NEW ) {
		lock
		cout<<"NIT: "<<getID()<<" state mismatch state="<<state<<endl;
		unlock
		return;
	}

	cout<<"--------PCB::waitToComplete()-----------"<<endl;
	cout<<"running: "<<PCB::runnig->getID()<<" je blokirana"<<endl;
	cout<<"----------------------------------------"<<endl;
	runnig->state = BLOCKED;
	blockedOnThread->insert(runnig);

	unlock

	System::dispatch();


}

void PCB::wrapper(){
	PCB::runnig->myThread->run();

	lock

	PCB* temp = runnig->blockedOnThread->popFront();

	for(temp;temp!=0;temp=runnig->blockedOnThread->popFront()){
		temp->state = READY;
		Scheduler::put(temp);
	}
	PCB::runnig->state = PCB::FINISHED;
	unlock
	System::dispatch();
}
