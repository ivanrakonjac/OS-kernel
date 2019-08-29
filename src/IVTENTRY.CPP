#include "IVTENTRY.H"
#include "KRNELEV.H"
#include "SYSTEM.H"

IVTEntry::IVTEntry(IVTNo num,void interrupt(*newEvent)(...)){
	asm cli
	this->oldInterrupt = 0;

	oldInterrupt = getvect(num);
	setvect(num,newEvent);

	this->ivtNumber = num;
	this->kernEvent = 0;

	System::interruptEntries[num] = this;

	asm sti
}

IVTEntry::~IVTEntry(){
	asm cli
	setvect(ivtNumber,oldInterrupt);
	System::interruptEntries[ivtNumber] = 0;
	asm sti
}

void IVTEntry::signal(IVTNo num){
	if(System::interruptEntries[num]->kernEvent!=0)
		System::interruptEntries[num]->kernEvent->signal();
}

void IVTEntry::useOld(IVTNo num){
	System::interruptEntries[num]->oldInterrupt();
}
