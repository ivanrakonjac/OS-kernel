//PCB.CPP

#include "PCB.H"
#include "SYSTEM.H"
#include "THREAD.H"
#include "PCBLIST.H"
#include "SCHEDULE.H"
//#include "SIGNLIST.H"
//#include "EXEWAIT.H"

int PCB::ajdi=0;
PCB* PCB::runnig= new PCB();
PCB* PCB::main = PCB::runnig;

PCB::PCB(){
	state=PCB::READY;
	quantum = 1;
	id=-1;
	myThread=0;
	blockedOnThread=0;
	//signalWoken = 0;

	stack=0;

	sp=0;
	ss=0;
	bp=0;

	//signalCounter=0;
	//signalsWaitngForExecution=0;
	owner=0;
	/*for(int brojac=0;brojac<16;++brojac){
		signalArray[brojac]=0;
		signalBlockArray[brojac]=0;
	}*/
}

PCB::PCB(unsigned long stackSize, unsigned int timeSlice, Thread* myTr){

		this->state=PCB::NEW;
		this->quantum=timeSlice;
		this->id=++ajdi;
		//this->signalWoken = 0;
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

		//signalCounter=0;
		//this->signalsWaitngForExecution = new ExecutionWaitingList();
		this->owner = PCB::runnig;
		/*for(int brojac=0;brojac<16;++brojac){
			signalArray[brojac]=0;
			signalBlockArray[brojac]=0;
		}*/

		/*if(owner!=0){
			for(int brojac=0;brojac<16;++brojac){
				signalBlockArray[brojac]=owner->signalBlockArray[brojac];
			}
		}*/

		asm sti;
}

PCB::~PCB(){
	asm cli
	//delete signalsWaitngForExecution;
	delete blockedOnThread;

	System::allThreads->remove(this);
	if(this->stack)
		delete [] this->stack;
	asm sti
}

void PCB::start(){
	lock
	if(this->state == PCB::NEW){
		if(this != System::idleThread->myPCB){
			this->state = PCB::READY;
			Scheduler::put((PCB*)this);
			System::numberOfUserThreads++;
		}
	}
	unlock
}

void PCB::waitToComplete(){
	lock
	if(this->myThread == System::idleThread->myPCB || this==runnig){
		unlock
		return;
	}
	if(this->state == FINISHED || this->state == NEW ) {
		unlock
		return;
	}
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

		if(temp!=PCB::main) Scheduler::put(temp);
	}

	/*if (PCB::runnig->owner!=0) {
		PCB::runnig->owner->signal(1);
	}

	PCB::runnig->signal(2);*/

	PCB::runnig->state = PCB::FINISHED;

	System::numberOfUserThreads--;

	unlock
	System::dispatch();
}
/*
void PCB::blockSignal(SignalId id){
	signalBlockArray[id]=1;
}
void PCB::unblockSignal(SignalId id){
	signalBlockArray[id]=0;
}

void PCB::registerHandler(SignalId id,SignalHandler sh){
	if(signalArray[id]==0){
		signalArray[id] = new SignalHandlerList();
		signalArray[id]->addToTheEnd(sh);
	}
	else{
		signalArray[id]->addToTheEnd(sh);
	}
}

void PCB::swap(SignalId id,SignalHandler sh1,SignalHandler sh2){
	if(signalArray[id]==0) return;
	signalArray[id]->swap(sh1,sh2);
}

void PCB::unregisterAllHandlers(SignalId id){
	if(signalArray[id]==0) return;
	signalArray[id]->~SignalHandlerList();
}

void PCB::signal(SignalId id){
	if(id==1 || id==2) return; //sistemski signali

	this->signalsWaitngForExecution->add(id);
	this->signalCounter+=1;
}

void PCB::updateSignalWaitgList(){
	int cntr2=0;
	for(int i=0;i<signalCounter;++i){
		unsigned id = signalsWaitngForExecution->popFirst();


		if(System::globallyBlockedSignals[id]==1){

			signalsWaitngForExecution->add(id);
			++cntr2;
		}
		else if(signalBlockArray[id] == 1){
			signalsWaitngForExecution->add(id);
			++cntr2;
		}
		else if(id==0){
			this->state = PCB::FINISHED;
			System::dispatch();
		}
		else{
			SignalHandlerList* shl = signalArray[id];
			SignalHandlerList::SignalHandlerNode* temp = shl->first;

			while(temp){
				temp->signalHandler();
				temp = temp->n;
			}

		}

	}

	signalCounter = cntr2;
}*/
