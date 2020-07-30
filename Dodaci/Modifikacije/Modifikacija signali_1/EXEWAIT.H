#ifndef _EXE_WAIT_SIG_H
#define _EXE_WAIT_SIG_H

#include "PCB.H"

class ExecutionWaitingList{

	class ExecNode{
	public:
		int val;
		ExecNode* next;
		ExecNode(int v,ExecNode* n=0){
			this->val = v;
			this->next = n;
		}
	};

public:
	ExecutionWaitingList(PCB* pcb){
		lock
		this->myPcb=pcb;
		this->numOfSignalsWaitForExec=0;

		this->first = 0;
		this->last = 0;
		unlock
	}
	~ExecutionWaitingList();

	void add(unsigned id);
	unsigned popFirst();
	void updateList();

	int getNumOfSignalsWaitForExec(){
		return this->numOfSignalsWaitForExec;
	}
	void incNumOfSignalsWaitForExec(){
		this->numOfSignalsWaitForExec+=1;
	}
	void decNumOfSignalsWaitForExec(){
		this->numOfSignalsWaitForExec-=1;
	}

	void stampaj();

	ExecNode* getFirst(){
		return this->first;
	}

protected:
	void addWithoutIncrement(unsigned id);

private:
	PCB* myPcb;
	unsigned numOfSignalsWaitForExec;
	ExecNode* first;
	ExecNode* last;

};

#endif
