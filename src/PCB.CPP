//PCB.CPP

#include "PCB.H"
#include "SYSTEM.H"
#include "THREAD.H"
#include "PCBLIST.H"
#include "SCHEDULE.H"

int PCB::ajdi=0;
PCB* PCB::runnig=0;

PCB::PCB(){
	state=PCB::READY;
	quantum = 0;
	id=-1;
	myThread=0;

	stack=0;

	sp=0;
	ss=0;
	bp=0;
}

PCB::PCB(unsigned long stackSize, unsigned int timeSlice, Thread* myTr){

		this->state=PCB::NEW;
		this->quantum=timeSlice;
		this->id=++ajdi;
		this->myThread=myTr;

		asm cli;

		if(stackSize>65536) stackSize = 65536;
		StackSize realStackSize = stackSize/sizeof(unsigned int);

		stack = new unsigned [stackSize/sizeof(unsigned int)];
		stack[stackSize-1] = 0x200;

		stack[stackSize-2] = FP_SEG(PCB::wrapper);
		stack[stackSize-3] = FP_OFF(PCB::wrapper);

		sp = FP_OFF(stack+stackSize-12);
		ss = FP_SEG(stack+stackSize-12);
		bp = FP_OFF(stack+stackSize-12);

		//thread = body;

		System::allThreads->insert(this);

		asm sti;
}

PCB::~PCB(){
	System::allThreads->remove(this);
	if(this->stack)
		delete [] this->stack;
}

void PCB::start(){
	lock
	if(this->state == PCB::NEW){

		this->state = PCB::READY;
		Scheduler::put((PCB*)this);

	}
	unlock
}

void PCB::waitToComplete(){

}

void PCB::wrapper(){
	PCB::runnig->myThread->run();
	lock
	PCB::runnig->state = PCB::FINISHED;
	unlock
	System::dispatch();
}
