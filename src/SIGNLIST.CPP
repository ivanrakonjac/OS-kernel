#include "SIGNLIST.H"
#include <iostream.h>
#include "System.h"

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

void SignalHandlerList::unregisterAllHandlers(unsigned id){
	cout<<"Ukloni hendlere za "<<id<<endl;

	while(SignalHandlerList::first){
		SignalHandlerNode* temp = SignalHandlerList::first;
		SignalHandlerList::first=SignalHandlerList::first->n;
		delete temp;
	}
}

void SignalHandlerList::stampaj(){
	SignalHandlerNode* temp = SignalHandlerList::first;
	while(temp){
		lock
		cout<<*(temp->signalHandler)<<" ";
		unlock
		temp = temp->n;
	}
	lock
	cout<<endl;
	unlock
}

