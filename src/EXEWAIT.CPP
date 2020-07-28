#include "EXEWAIT.H"
#include "SYSTEM.H"
#include "SIGNLIST.H"

void ExecutionWaitingList::add(unsigned id){
lock
	ExecNode* temp = new ExecNode(id);

	if(this->first == 0){
		this->first = temp;
		this->last = this->first;
	}
	else {
		this->last->next = temp;
		this->last = this->last->next;
	}

	this->numOfSignalsWaitForExec++;
unlock
}

void ExecutionWaitingList::addWithoutIncrement(unsigned id){
lock
	ExecNode* temp = new ExecNode(id);

	if(this->first == 0){
		this->first = temp;
		this->last = this->first;
	}
	else {
		this->last->next = temp;
		this->last = this->last->next;
	}

unlock
}

unsigned ExecutionWaitingList::popFirst(){
	if(this->first == 0) return -1;

	ExecNode* temp = this->first;

	this->first = this->first->next;

	if(this->first == 0) this->last = 0;

	unsigned x = temp->val;

	delete temp;

	return x;
}

void ExecutionWaitingList::updateList(){
lock

	int brojSignalaKojiSuBlokirani=0;

	while(this->numOfSignalsWaitForExec!=0){

		unsigned val=this->popFirst();

		//-1 ako nema nista u listi
		//a i za svaki slucaj ako neko zeli da ubaci negativne brojeve
		if(val<0){
			unlock
			return;
		}
		//cout<<"Val="<<val<<endl;;


		if(System::globallyBlockedSignals[val]==0 && this->myPcb->signalBlockArray[val]==0){


			this->numOfSignalsWaitForExec=this->numOfSignalsWaitForExec-1;

			SignalHandlerList* shl = myPcb->signalArray[val];

			//cout<<"Lista "<<val<<endl;
			cout<<"Lista "<<val<< " idNiti:"<<this->myPcb->id<<endl;

			if(shl!=0){
				SignalHandlerList::SignalHandlerNode* temp = shl->first;

				while(temp){
					temp->signalHandler();
					temp=temp->n;
				}
			}
		}
		else{
			this->numOfSignalsWaitForExec=this->numOfSignalsWaitForExec-1;

			this->addWithoutIncrement(val);
			brojSignalaKojiSuBlokirani=brojSignalaKojiSuBlokirani+1;
		}

	}

	this->numOfSignalsWaitForExec=this->numOfSignalsWaitForExec+brojSignalaKojiSuBlokirani;

unlock
}


ExecutionWaitingList::~ExecutionWaitingList(){

	/*if(this->myPcb->id==-1){
		this->first=this->last=0;
		this->myPcb=0;
		this->numOfSignalsWaitForExec=0;
		return;
	}*/

	if(PCB::pcbNum==2) return;

	if(this->numOfSignalsWaitForExec==0){
		this->first=this->last=0;
		this->myPcb=0;
		this->numOfSignalsWaitForExec=0;
		return;
	}else if(this->numOfSignalsWaitForExec>0){

		cout<<"Usao sam u destruktor brSIgnala:"<<this->numOfSignalsWaitForExec <<" idNiti:"<<this->myPcb->id<<endl;


		for (int brojac = 0; brojac < this->numOfSignalsWaitForExec; brojac++) {
			unsigned val=this->popFirst();

			if(val<0){
				unlock
				return;
			}

			//cout<<"vrednost lokalnog blok niza"<<this->myPcb->signalBlockArray[val]<<endl;

			if(System::globallyBlockedSignals[val]==0 && this->myPcb->signalBlockArray[val]==0){

				SignalHandlerList* shl = myPcb->signalArray[val];

				if(shl!=0){
					SignalHandlerList::SignalHandlerNode* temp = shl->first;

					while(temp){
						temp->signalHandler();
						temp=temp->n;
					}
				}
			}else{
				cout<<"Blokirano "<<val<<endl;
				if(val==65535) return;
			}
		}
		this->first=this->last=0;
		this->myPcb=0;
		this->numOfSignalsWaitForExec=0;

	}

}

void ExecutionWaitingList::stampaj(){
	ExecNode* temp=first;

	while(temp){
		lock
		cout<<temp->val<<" ";
		unlock
		temp=temp->next;
	}
	lock
	cout<<endl;
	unlock
}
