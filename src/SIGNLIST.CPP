#include "SIGNLIST.H"

void SignalHandlerList::addToTheEnd(SignalHandler sh){
	SignalHandlerNode* newNode = new SignalHandlerNode(sh);

	if(first == 0){
		first = newNode;
		last = first;
	}
	else{
		last->n = newNode;
		last = last->n;
	}
}

SignalHandlerList::~SignalHandlerList(){
	SignalHandlerNode* temp;

	while(first){
		temp = first;
		first = first->n;
		delete temp;
	}
}

void SignalHandlerList::swap(SignalHandler sh1,SignalHandler sh2){

	SignalHandlerNode* prev1=0;
	SignalHandlerNode* current1=first;

	while(current1 && current1->signalHandler!=sh1){
		prev1=current1;
		current1=current1->n;
	}

	SignalHandlerNode* prev2=0;
	SignalHandlerNode* current2=first;

	while(current2 && current2->signalHandler!=sh2){
		prev2=current2;
		current2=current2->n;
	}


	if(current1==0 || current2==0) return;

	if(prev1!=0){
		prev1->n = current2;
	}
	else{
		first = current2;
	}

	if(prev2!=0){
		prev2->n = current1;
	}
	else{
		first = current1;
	}

	SignalHandlerNode* temp = current2->n;
	current2->n = current1->n;
	current1->n = temp;

	return;
}
