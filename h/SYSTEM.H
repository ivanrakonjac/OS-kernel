#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <iostream.h>
#include <DOS.H>

class PCB_LIST;
class Thread;
class TIME_BLCK_LIST;
class IVTEntry;

typedef void interrupt (*pInterrupt)(...); //pok na prekidnu rutinu
void interrupt timer(...);

#define lock System::lockFlag=0;

#define unlock System::lockFlag = 1; \
if(System::context_switch_on_demand) System::dispatch();

class System{

public:

	static PCB_LIST* allThreads;
	static Thread* idleThread;
	static TIME_BLCK_LIST* timeBlockList;
	static int tstBrojac;

	static volatile unsigned int runningTimeCntr;
	static volatile unsigned int lockFlag;
	static volatile unsigned int context_switch_on_demand;
	static IVTEntry* interruptEntries[256];

	static void inic();
	static void restore();
	static void dispatch();
};

#endif
