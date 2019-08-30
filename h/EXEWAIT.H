#ifndef _EXE_WAIT_SIG_H
#define _EXE_WAIT_SIG_H

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
	ExecutionWaitingList(){
		this->first = 0;
		this->last = 0;
	}
	~ExecutionWaitingList();

	void add(unsigned id);
	unsigned popFirst();
	void updateList();

	ExecNode* first;
	ExecNode* last;
};

#endif
