#ifndef _KERNEL_SEM_
#define _KERNEL_SEM_

class PCB_LIST;
class TIME_BLCK_LIST;

class KernelSem{
public:
	KernelSem(int v);
	~KernelSem();

	int val() const;
	int wait(int maxTimeToWait);
	int signal(int n);

	void incVal();

private:
	PCB_LIST* blockList;
	int value;
};

#endif
