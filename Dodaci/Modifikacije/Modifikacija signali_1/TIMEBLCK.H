#ifndef _TIME_BLOCK_LIST_H
#define _TIME_BLOCK_LIST_H

#include "SEMAPHOR.H"

class PCB;
class KernelSem;

class TIME_BLCK_LIST{

	class timeNode{
		public:
			PCB* data;
			KernelSem* semaphor;
			Time time;
			timeNode* next;
			timeNode(PCB* d,Time t,KernelSem* sem,timeNode* next=0){
				this->data = d;
				this->semaphor = sem;
				this->time = t;
				this->next = next;
			}

			void decTime(){
			    --time;
			}
	};

public:

	TIME_BLCK_LIST(){
		this->first = 0;
		this->last = 0;
	}
	~TIME_BLCK_LIST();

	int insert(PCB* pcb,Time maxTime,KernelSem* sem);

	timeNode* popFront();

	void update();
	void stampaj();

	timeNode* first;
	timeNode* last;

};

#endif
