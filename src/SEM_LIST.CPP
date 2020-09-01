#include "SEM_LIST.H"
#include "KRNELSEM.H"
#include "SCHEDULE.H"
#include "SYSTEM.H"
#include "TIMEBLCK.H"
#include "PCBLIST.H"

SEM_LIST::SemNode* SEM_LIST::first=0;
SEM_LIST::SemNode* SEM_LIST::last=0;


int SEM_LIST::insert(KernelSem* kernelSem){
	lock
	SemNode* newNode = new SemNode (kernelSem);

	if(SEM_LIST::first == 0){
		SEM_LIST::first = newNode;
		SEM_LIST::last=SEM_LIST::first;
	}
	else{
		SEM_LIST::last->next = newNode;
		SEM_LIST::last = SEM_LIST::last->next;
	}
	unlock
	return 1;
}

int SEM_LIST::remove(KernelSem* kernelSem){
	lock
	SemNode* temp = SEM_LIST::first;
	SemNode* prev = 0;


	while(temp != 0 && (temp->kernelSem != kernelSem)){
		prev=temp;
		temp = temp->next;
	}

	if(temp == 0) {
		unlock
		return 0;
	}

	if(prev == 0){
		SEM_LIST::first = temp->next;
	}
	else{
		prev->next = temp->next;
	}

	if(temp->next == 0){
		SEM_LIST::last = prev;
	}

	delete temp;
	unlock
	return 1;
}

KernelSem* SEM_LIST::popFront(){
	lock
	if(SEM_LIST::first == 0){
		unlock
		return 0;
	}

	SemNode* temp = SEM_LIST::first;

	SEM_LIST::first = SEM_LIST::first->next;

	if(SEM_LIST::first == 0) SEM_LIST::last = 0;

	KernelSem* kernelSem = temp->kernelSem;

	delete temp;

	unlock
	return kernelSem;
}

void SEM_LIST::stampaj(){
	SemNode* temp = SEM_LIST::first;
	while(temp){
		lock
		cout<<temp->kernelSem->val()<<" ";
		temp = temp->next;
		unlock
	}
	lock
	cout<<endl;
	unlock
}

SEM_LIST::~SEM_LIST(){

	while(SEM_LIST::first){

		KernelSem* temp = this->popFront();

		if(temp==0) break;

		//delete temp;
	}
	SEM_LIST::first=SEM_LIST::last=0;

}

void SEM_LIST::updateTimeBlockedThreads(){
	SemNode* temp = SEM_LIST::first;

	while(temp){
		temp->kernelSem->update();
		temp=temp->next;
	}
}


