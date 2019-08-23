#include "PCBLIST.H"
#include "PCB.H"

int PCB_LIST::insert(PCB* pcb){
	Node* newNode = new Node (pcb);

	if(first == 0){
		first = newNode;
		last=first;
	}
	else{
		last->next = newNode;
		last = last->next;
	}

	return 1;
}

int PCB_LIST::remove(PCB* pcb){
	Node* temp = first;
	Node* prev = 0;

	while(temp != 0 && temp->node != pcb){
		prev=temp;
		temp = temp->next;
	}

	if(temp == 0) return 0;

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

	return 1;
}

PCB* PCB_LIST::getPCBwithID(int id){
	if(first == 0) return 0;
	if(id<0) return 0;

	Node* temp;

	for(temp = first ; temp != 0; temp = temp->next){
		if(temp->node->getID() == id) return temp->node;
	}

	if(temp == 0) return 0;

}

PCB* PCB_LIST::popFront(){
	if(first == 0) return 0;

	Node* temp = first;

	first = first->next;

	if(first == 0) last = 0;

	PCB* pcb = temp->node;

	delete temp;

	return pcb;
}

PCB_LIST::~PCB_LIST(){
	Node* temp=0;

	while(first){
		temp = first;
		first = first->next;
		delete temp;
	}
}


