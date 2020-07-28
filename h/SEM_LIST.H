#ifndef _SEM_LIST_H
#define _SEM_LIST_H

class KernelSem;

class SEM_LIST{
public:

	class SemNode{
		public:
			KernelSem* kernelSem;
			SemNode* next;
			SemNode(KernelSem* kernelSem,SemNode* next=0){
				this->kernelSem = kernelSem;
				this->next = next;
			}
			~SemNode(){
			}
	};

public:

	SEM_LIST(){
		this->first = 0;
		this->last = 0;
	}
	~SEM_LIST();

	static void updateTimeBlockedThreads();

	int insert(KernelSem* kerSem);
	int remove(KernelSem* kerSem);

	KernelSem* popFront();
	void stampaj();

	static SemNode* first;
	static SemNode* last;

};

#endif
