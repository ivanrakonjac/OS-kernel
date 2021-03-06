//PCB.CPP

#include "PCB.H"
#include "SYSTEM.H"
#include "THREAD.H"
#include "PCBLIST.H"
#include "SCHEDULE.H"
#include <iostream.h>
#include "SIGNLIST.H"
#include "EXEWAIT.H"

#include <iostream.h>

int PCB::ajdi=0;
PCB* PCB::runnig= new PCB();
PCB* PCB::main = PCB::runnig;
int PCB::pcbNum=0;

PCB::PCB(){
	state=PCB::READY;
	quantum = 1;
	id=-1;
	myThread=0;
	blockedOnThread=0;
	killedBySignal = 0;
	timeUnblock=0;

	PCB::pcbNum=PCB::pcbNum+1;

	stack=0;

	sp=0;
	ss=0;
	bp=0;

	signalsWaitngForExecution=0;
	owner=0;

	for(int brojac=0;brojac<16;++brojac){
		signalArray[brojac]=0;
		signalBlockArray[brojac]=0;
	}

	/*
	 * dodato zbog modifikacije
	 * setuje matricu maski za signal na 0
	 */
	for(int brojac2=0; brojac2<5;brojac2++){
		matricaMaskiSignala[brojac][brojac2]=0;
	}
}

PCB::PCB(unsigned long stackSize, unsigned int timeSlice, Thread* myTr){

		this->state=PCB::NEW;
		this->quantum=timeSlice;
		this->id=++ajdi;
		this->killedBySignal = 0;
		this->myThread=myTr;
		this->blockedOnThread = new PCB_LIST();
		this->timeUnblock=0;

		PCB::pcbNum=PCB::pcbNum+1;

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

		this->signalsWaitngForExecution = new ExecutionWaitingList(this);
		this->owner = PCB::runnig;
		cout<<"this: "<<this->id<<" owner: "<<owner->id<<endl;

		for(int brojac=0;brojac<16;++brojac){
			signalArray[brojac]=0;
			signalBlockArray[brojac]=0;

			/*
			 * dodato zbog modifikacije
			 * setuje matricu maski za signal na 0
			 */
			for(int brojac2=0; brojac2<5;brojac2++){
				matricaMaskiSignala[brojac][brojac2]=0;
			}

		}

		//Nit koja se kreira nasledjuje sva podesavanja, vezan za signale, od niti koja ju je napravila
		if(owner!=0){
			for(int brojac=0;brojac<16;++brojac){
				signalBlockArray[brojac]=owner->signalBlockArray[brojac];
				if(this->owner->signalArray[brojac]!=0){
					this->signalArray[brojac]=owner->signalArray[brojac];
					//cout<<"this: "<<this->id<<" "<<this->signalArray[brojac]<<" owner: "<<owner->signalArray[brojac]<<endl;
				}
			}
		}

		asm sti;
}

PCB::~PCB(){
	asm

	if(this->killedBySignal==0){
		delete signalsWaitngForExecution;
		delete blockedOnThread;
	}

	System::allThreads->remove(this);

	PCB::pcbNum=PCB::pcbNum-1;

	//cout<<"Sklonjena iz liste - "<<System::allThreads->remove(this)<<endl;

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
		Scheduler::put(temp);
	}

	if (PCB::runnig->owner!=0 && PCB::runnig->owner->signalArray[1]!=0) {
		//cout<<"PCB::runnig->owner->id "<<PCB::runnig->owner->id<<" "<<PCB::runnig->owner->signalArray[1]<<endl;
		PCB::runnig->owner->systemSignal(1);
	}

	PCB::runnig->systemSignal(2);

	PCB::runnig->state = PCB::FINISHED;

	System::numberOfUserThreads--;

	unlock
	System::dispatch();
}

void PCB::blockSignal(SignalId id){
	signalBlockArray[id]=1;
}
void PCB::unblockSignal(SignalId id){
	signalBlockArray[id]=0;
}

void PCB::registerHandler(SignalId id,SignalHandler sh[],int maska){

	int pomeraj=4; //jer imamo max 5 hendlera po signalu
	while(maska!=0 || pomeraj>0){
		if(maska&1==1){
			matricaMaskiSignala[id][pomeraj]=1;
		}
		else{
			matricaMaskiSignala[id][pomeraj]=0;
		}

		maska=maska>>1;
		pomeraj--;
	}

	cout<<"Maska za signal:"<<id<<endl;
	for(int brojac2=0; brojac2<5;brojac2++){
		cout<< matricaMaskiSignala[id][brojac2]<<" ";;
	}
	cout<<endl;


	if(signalArray[id]==0){
		cout<<"signalArray[id]==0"<<endl;
		signalArray[id] = new SignalHandlerList();
		for (int var = 0; var < 5; var++) {
			signalArray[id]->addToTheEnd(sh[var]);
		}

	}
	else{
		cout<<"signalArray[id]!=0"<<endl;
		for (int var = 0; var < 5; var++) {
			signalArray[id]->addToTheEnd(sh[var]);

		}
	}

}

void PCB::swap(SignalId id,SignalHandler sh1,SignalHandler sh2){
	cout<<"PCB::swap"<<endl;
	//if(signalArray[id]==0) return;
	signalArray[id]->swap(sh1,sh2);
}

void PCB::unregisterAllHandlers(SignalId id){
	this->signalArray[id]->unregisterAllHandlers(id);
}

void PCB::signal(SignalId id){

	lock

	//signali 1 i 2 su sistemski signali
	if(id==1 || id==2){
		unlock
		return;
	}

	//za hendlovanje signalom 0
	if(id==0){
		cout<<"SIGNAL0 this->myPcb->id "<<this->id<<endl;

		//odblokiram sve niti zabodene na ovoj i obrisem listu
		delete this->blockedOnThread;

		//cout<<"owner:"<<this->owner->id<<endl;
		//cout<<"this->owner->signalArray[1] = "<<this->owner->signalArray[1]<<endl;


		if (this->owner!=0 && this->owner->signalArray[1]!=0) {
			//cout<<"Poslat signal 1 "<<endl;
			this->owner->systemSignal(1);
		}

		this->systemSignal(2);

		this->state = PCB::FINISHED;
		this->killedBySignal=1;

		this->signalsWaitngForExecution->stampaj();

		delete this->signalsWaitngForExecution;

		this->signalsWaitngForExecution->stampaj();

		System::allThreads->stampaj();

		//cout<<"Sklonjena iz liste - "<<System::allThreads->remove(this)<<endl

		//System::zaTestiranje=1;

		unlock
		System::dispatch();
		return;
	}

	if(this==PCB::main && this->signalsWaitngForExecution==0){
		this->signalsWaitngForExecution = new ExecutionWaitingList(PCB::main);
	}

	if(this->signalsWaitngForExecution!=0){
		this->signalsWaitngForExecution->add(id);
	}

	unlock
	return;

}

//Fja samo za slanje signala 1 i 2
void PCB::systemSignal(SignalId id){

	//cout<<"Usao sam u systSign "<<id<<endl;

	if(id==1 || id==2){
		if(this==PCB::main && this->signalsWaitngForExecution==0){
			//cout<<"Napravio sam listu"<<endl;
			this->signalsWaitngForExecution = new ExecutionWaitingList(PCB::main);
		}

		if(this->signalsWaitngForExecution!=0){
			//cout<<"Dodao sam"<<endl;
			this->signalsWaitngForExecution->add(id);
		}
	}else{
		return;
	}



}
