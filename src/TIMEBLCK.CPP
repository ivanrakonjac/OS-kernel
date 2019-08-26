#include "TIMEBLCK.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "KRNELSEM.H"
#include <iostream.h>

int TIME_BLCK_LIST::insert(PCB* pcb,Time maxTime,KernelSem* sem){

    if(maxTime<0) return 0;

	timeNode* newNode = new timeNode (pcb,maxTime,sem);
	timeNode* temp = first;
	timeNode* prev = 0;

	if(first == 0){
		first = newNode;
		last=first;

		return 1;
	}


	if(newNode->time <= first->time){
	    newNode->next = first;
	    first = newNode;
	    first->next->time -= newNode->time;

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

	        return 1;
	    }
	    else{
	        prev->next = newNode;
	        newNode->next = 0;
	        last = newNode;

	        return 1;
	    }

	    return 1;
	}
}

TIME_BLCK_LIST::timeNode* TIME_BLCK_LIST::popFront(){
	if(first == 0) return 0;

	timeNode* temp = first;

	first = first->next;

	if(first == 0) last = 0;

	return temp;
}

TIME_BLCK_LIST::~TIME_BLCK_LIST(){
	timeNode* temp=0;

	while(first){
		temp = first;
		first = first->next;
		delete temp;
	}
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
	first->decTime();
	while(first && first->time==0){

		timeNode* temp =this->popFront();

		temp->data->setState(PCB::READY);
		temp->semaphor->incVal();

		cout<<"SIGNAL(n): val = "<<temp->semaphor->val()<<endl;
		cout<<"PROBUDJENA NIT: "<<temp->data->getID()<<endl;

		Scheduler::put(temp->data);
	}
}
