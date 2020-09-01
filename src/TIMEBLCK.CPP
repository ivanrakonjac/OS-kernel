#include "TIMEBLCK.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "KRNELSEM.H"
#include "SYSTEM.H"
#include <iostream.h>
/*
int TIME_BLCK_LIST::insert(PCB* pcb,Time maxTime,KernelSem* sem){
	lock

	timeNode* newNode = new timeNode (pcb,maxTime,sem);
	timeNode* temp = first;
	timeNode* prev = 0;

	if(first == 0){
		first = newNode;
		last=first;

		unlock
		return 1;
	}

	if(newNode->time <= first->time){
		newNode->next = first;
		first = newNode;

		unlock
		return 1;
	}

	if(newNode->time>=last->time){
		last->next=newNode;
		last=newNode;

		unlock
		return 1;
	}

	while(first->next->time<maxTime){
		temp=temp->next;
	}
	newNode->next=temp->next;
	temp->next=newNode;

	unlock
	return 1;
}

void TIME_BLCK_LIST::update(){
	lock

	if(first==0){
		unlock
		return;
	}

	timeNode* temp=first;
	timeNode* old=0;

	while(temp!=0){
		temp->decTime();
		temp=temp->next;
	}

	while(first->time==0){
		old=first;
		first=first->next;
		if(first==0) last=0;
		old->data->setState(PCB::READY);
		if(old->data!=PCB::main) Scheduler::put(old->data);

		delete old;
	}

	unlock
}

TIME_BLCK_LIST::~TIME_BLCK_LIST(){
	asm cli;
	timeNode* temp=0;

	while(first){
		temp = first;
		first = first->next;
		delete temp;
	}
	asm sti;
}
*/



int TIME_BLCK_LIST::insert(PCB* pcb,Time maxTime,KernelSem* sem){
	lock

    if(maxTime<0){
    	unlock
    	return 0;
    }

	timeNode* newNode = new timeNode (pcb,maxTime,sem);
	timeNode* temp = first;
	timeNode* prev = 0;

	if(first == 0){
		first = newNode;
		last=first;

		unlock
		return 1;
	}


	if(newNode->time <= first->time){
	    newNode->next = first;
	    first = newNode;
	    first->next->time -= newNode->time;

	    unlock
	    return 1;
	}

	if(newNode->time > first->time){
	    temp = first;
	    while(newNode->time >= temp->time){
	        newNode->time -= temp->time;
	        prev = temp;
	        if(temp->next != 0)temp = temp->next;
	        else {temp = 0; break;}
	    }
	    if(temp != 0){
	        prev->next = newNode;
	        newNode->next = temp;
	        temp->time -= newNode->time;

	        unlock
	        return 1;
	    }
	    else{
	        prev->next = newNode;
	        newNode->next = 0;
	        last = newNode;

	        unlock
	        return 1;
	    }

	    unlock
	    return 1;
	}
}

TIME_BLCK_LIST::timeNode* TIME_BLCK_LIST::popFront(){

	lock
	if(first == 0){
		//cout<<"PRAZNAAA"<<endl;
		unlock
		return 0;
	}

	timeNode* temp = first;

	first = first->next;

	if(first == 0){
		last = 0;
	}

	unlock
	return temp;
}

TIME_BLCK_LIST::~TIME_BLCK_LIST(){
	asm cli;

	while(first){

		timeNode* temp=this->popFront();

		//ako je ostala neka nit zablokirana
		/*PCB* pcb=temp->data;
		if(pcb->getState()==PCB::BLOCKED){
			pcb->setState(PCB::READY);
			Scheduler::put(pcb);
		}*/

		delete temp;
	}

	first=last=0;

	asm sti;
}

void TIME_BLCK_LIST::stampaj(){
    timeNode* temp = first;
    while(temp){
        cout<<temp->time<<" ";
        temp = temp->next;
    }
    cout<<endl;
}

void TIME_BLCK_LIST::update(){
	//lock
	if(first==0){
		//cout<<"Ovde nema nikog"<<endl;
		//unlock
		return;
	}
	first->decTime();
	while(first && first->time==0){

		timeNode* temp =this->popFront();

		//povecam vrednost na sem jer sam pustio nit
		temp->semaphor->incVal();
		//setujem da je PCB READY
		temp->data->setState(PCB::READY);
		//setujem da je probudjen zbog isteka vremena
		temp->data->setTimeUnblock(1);

		//temp->data->setSignalWoken(0);
		//temp->semaphor->incVal();

		//cout<<"SIGNAL(n): val = "<<temp->semaphor->val()<<endl;
		//cout<<"PROBUDJENA NIT: "<<temp->data->getID()<<endl;

		Scheduler::put(temp->data);
	}
	//unlock
}
