#include "KRNELSEM.H"
#include "TIMEBLCK.H"
#include "PCBLIST.H"
#include "SYSTEM.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "SEM_LIST.H"

KernelSem::KernelSem(int inic){
	asm cli;
	value = inic;
	blockList = new PCB_LIST();
	timeBlockList=new TIME_BLCK_LIST();
	System::allSem->insert(this);
	asm sti;

	/*lock
	cout<<"Dodat sem sa vredn: "<<value<<endl;
	unlock*/
}

KernelSem::~KernelSem(){
	asm cli;
	delete blockList;
	delete timeBlockList;
	System::allSem->remove(this);
	asm sti;
}

int KernelSem::wait(int maxTimeToWait){

	asm cli;

	if(maxTimeToWait == 0){
		value = value - 1;

		if(value>=0){
			asm sti;
			return 1;
		}

		PCB::runnig->setState(PCB::BLOCKED);
		blockList->insert(PCB::runnig);

		//cout<<"Dodata u blockList"<<endl;
	}
	else{
		PCB::runnig->setState(PCB::BLOCKED);
		value=value-1;
		this->timeBlockList->insert(PCB::runnig,maxTimeToWait,this);
	}

	asm sti;

	System::dispatch();


	/*if(!PCB::runnig->getSignalWoken()){
		PCB::runnig->setSignalWoken(0);
		return 0;
	}*/

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

	/*lock
	cout<<"SIGNAL "<<n<<endl;
	unlock*/

	lock

	if(n<0){
		unlock
		return n;
	}
	else if(n == 0 ){
		value = value + 1;

		PCB* temp = blockList->popFront();

		if(temp==0){
			/*cout<<"Usao sam ovde je je blokcLista prazna"<<endl;
			cout<<"Temp first: "<<timeBlockList->first->data<<endl;*/
			TIME_BLCK_LIST::timeNode* tndp=timeBlockList->popFront();
			if(tndp==0){
				//cout<<"Vracam 0 i bezim"<<endl;
				unlock
				return 0;
			}
			temp=tndp->data;
			//cout<<"Temp: "<<temp<<endl;
		}

		if(temp == 0){
			cout<<"Nema nikoga"<<endl;
			unlock
			return n;
		}

		temp->setState(PCB::READY);
		//temp->setSignalWoken(1);

		Scheduler::put(temp);

		unlock
		//cout<<"Temp->PCB"<<temp<<endl;
		return 0;
	}
	else if(n>0){
		int cntr = 0;
		value = value + n;

		for (int i= 0; i < n; ++i) {
			PCB* temp = blockList->popFront();
			if(temp == 0){
				break;
			}

			cntr++;
			temp->setState(PCB::READY);
			//temp->setSignalWoken(1);

			Scheduler::put(temp);
		}


		for (int j= cntr; j < n; ++j) {

			this->timeBlockList->stampaj();

			TIME_BLCK_LIST::timeNode* temp2=timeBlockList->popFront();
			//cout<<temp2<<endl;

			if(temp2 == 0){
				unlock
				return cntr;
			}

			PCB* temp = temp2->data;

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
