#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <iostream.h>
#include <DOS.H>

class PCB_LIST;
class Thread;
class TIME_BLCK_LIST;
class IVTEntry;
class SEM_LIST;

typedef void interrupt (*pInterrupt)(...); //pok na prekidnu rutinu
void interrupt timer(...);

#define lock System::lockFlag=0;

#define unlock System::lockFlag = 1; \
if(System::context_switch_on_demand) System::dispatch();

class System{

public:

	static PCB_LIST* allThreads;
	static SEM_LIST* allSem;
	static Thread* idleThread;
	static int tstBrojac;

	static volatile unsigned int runningTimeCntr;
	static volatile unsigned int lockFlag;
	static volatile unsigned int context_switch_on_demand;
	static volatile int numberOfUserThreads;
	static IVTEntry *interruptEntries[256];
	static int globallyBlockedSignals[16];

	static volatile int zaTestiranje;

	static void inic();
	static void restore();
	static void dispatch();
};

#endif
