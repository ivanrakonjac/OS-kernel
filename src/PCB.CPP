//PCB.CPP

#include "PCB.H"
#include "SYSTEM.H"

int PCB::ajdi=0;
PCB* PCB::runnig=0;

PCB::PCB(){
	state=PCB::READY;
	quantum = 0;
	id=-1;
	thread=0;

	stack=0;

	sp=0;
	ss=0;
	bp=0;
}

PCB::PCB(unsigned long stackSize, unsigned int timeSlice, void (*body)()){

		state=PCB::NEW;
		quantum=timeSlice;
		id=++ajdi;

		asm cli;

		stack = new unsigned [stackSize];
		stack[stackSize-1] = 0x200;

		stack[stackSize-2] = FP_SEG(body);
		stack[stackSize-3] = FP_OFF(body);

		sp = FP_OFF(stack+1012);
		ss = FP_SEG(stack+1012);
		bp = FP_OFF(stack+1012);

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
	}
	unlock
}

void PCB::waitToComplete(){

}

void PCB::wraper(){

}
