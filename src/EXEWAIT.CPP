#include "EXEWAIT.H"

void ExecutionWaitingList::add(unsigned id){
	ExecNode* temp = new ExecNode(id);

	if(first == 0){
		first = temp;
		last =first;
	}
	else {
		last->next = temp;
		last = last->next;
	}
}

unsigned ExecutionWaitingList::popFirst(){
	if(first == 0) return 0;

	ExecNode* temp = first;

	first = first->next;

	if(first == 0) last = 0;

	unsigned x = temp->val;

	delete temp;

	return x;
}

ExecutionWaitingList::~ExecutionWaitingList(){
	ExecNode* temp;

	while(first){
		temp=first;
		first=first->next;
		delete temp;
	}
}
