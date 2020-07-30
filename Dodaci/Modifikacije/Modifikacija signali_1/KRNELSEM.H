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

	TIME_BLCK_LIST* getTimeBlockList(){
		return timeBlockList;
	}

	PCB_LIST* getBlockList(){
		return blockList;
	}

private:
	PCB_LIST* blockList;
	TIME_BLCK_LIST* timeBlockList;
	int value;
};

#endif
