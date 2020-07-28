#include "PCBLIST.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "SYSTEM.H"
#include <iostream.h>

int PCB_LIST::nodeNum=0;

int PCB_LIST::insert(PCB* pcb){
	lock
	Node* newNode = new Node (pcb);

	if(first == 0){
		first = newNode;
		last=first;
	}
	else{
		last->next = newNode;
		last = last->next;
	}
	unlock
	return 1;
}

int PCB_LIST::remove(PCB* pcb){
	lock
	Node* temp = first;
	Node* prev = 0;


	while(temp != 0 && (temp->node != pcb)){
		prev=temp;
		temp = temp->next;
	}

	if(temp == 0) {
		unlock
		return 0;
	}

	if(prev == 0){
		first = temp->next;
	}
	else{
		prev->next = temp->next;
	}

	if(temp->next == 0){
		last = prev;
	}

	delete temp;
	unlock
	return 1;
}

PCB* PCB_LIST::getPCBwithID(int id){
	lock
	if(first == 0){
		unlock
		return 0;
	}
	if(id<0){
		unlock
		return 0;
	}

	Node* temp;

	for(temp = first ; temp != 0; temp = temp->next){
		if(temp->node->getID() == id){
			unlock
			return temp->node;
		}
	}

	if(temp == 0){
		unlock
		return 0;
	}

}

PCB* PCB_LIST::popFront(){
	lock
	if(first == 0){
		last=0;
		unlock
		return 0;
	}

	Node* temp = first;

	first = first->next;

	if(first == 0) last = 0;

	PCB* pcb = temp->node;

	delete temp;

	unlock
	return pcb;
}

void PCB_LIST::stampaj(){
	Node* temp = first;
	while(temp){
		lock
		cout<<temp->node->getID()<<"-"<<temp->node->getState()<<"| " ;
		temp = temp->next;
		unlock
	}
	lock
	cout<<endl;
	unlock
}

PCB_LIST::~PCB_LIST(){

	while(first){


		PCB* pcb = this->popFront(); //u popFront funkciji mi se obrise Node*

		if(pcb==0){
			break;
		}

		//ako je ostala neka nit zablokirana
		if(pcb->getState()==PCB::BLOCKED){
			cout<<"Ubacio sam nit "<<pcb->getID()<<endl;
			pcb->setState(PCB::READY);
			Scheduler::put(pcb);
		}

	}
	first=last=0;
}


