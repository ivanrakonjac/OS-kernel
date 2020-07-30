#include "IVTENTRY.H"
#include "KRNELEV.H"
#include "SYSTEM.H"

IVTEntry::IVTEntry(unsigned numEntry,pInterrupt newRout){
	lock

	this->numEntry=numEntry;

	oldRout = getvect(numEntry);
	setvect(numEntry,newRout);

	this->kernEvent = 0;

	System::interruptEntries[this->numEntry]=this;

	unlock
}

IVTEntry::~IVTEntry(){
	asm cli;
	setvect(numEntry,oldRout);
	System::interruptEntries[this->numEntry] = 0;
	asm sti;

}

void IVTEntry::signal(){
	if(this->kernEvent!=0){
		kernEvent->signal();
	}
}

void IVTEntry::callOldRoutine(){
	this->oldRout();
}


