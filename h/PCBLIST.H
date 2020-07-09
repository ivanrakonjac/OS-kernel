#ifndef _PCB_LIST_H
#define _PCB_LIST_H

class PCB;

class PCB_LIST{
public:
	static int nodeNum;
	class Node{
		public:
			PCB* node;
			Node* next;
			Node(PCB* node,Node* next=0){
				this->node = node;
				this->next = next;
				PCB_LIST::nodeNum++;
			}
			~Node(){
				PCB_LIST::nodeNum--;
			}
	};

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
	void stampaj();

	Node* first;
	Node* last;

};

#endif
