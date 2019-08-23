#ifndef _PCB_LIST_H
#define _PCB_LIST_H

class PCB;

class PCB_LIST{
	class Node{
		public:
			PCB* node;
			Node* next;
			Node(PCB* node,Node* next=0){
				this->node = node;
				this->next = next;
			}
	};

	Node* first;
	Node* last;

public:

	PCB_LIST(){
		this->first = 0;
		this->last = 0;
	}
	~PCB_LIST();

	int insert(PCB* pcb);
	int remove(PCB* pcb);

	PCB* getPCBwithID(int ind);
	PCB* popFront();

};

#endif