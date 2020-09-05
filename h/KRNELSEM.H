#ifndef _KERNEL_SEM_
#define _KERNEL_SEM_

class PCB_LIST;
class TIME_BLCK_LIST;
class Queue;

class KernelSem{
public:
	KernelSem(int v);
	~KernelSem();

	int val() const;
	int wait(int maxTimeToWait);
	int signal(int n=0);

	void incVal();
	void update();

private:
	Queue* blockList;
	Queue* timeBlockList;
	int value;
};

#endif
