#include "System.h"
#include "SCHEDULE.H"
#include "PCBLIST.H"
#include "PCB.H"
#include "IDLE.H"
#include "TIMEBLCK.H"
#include "SEM_LIST.H"
#include "KRNELSEM.H"
#include "IVTENTRY.H"
#include "KRNELEV.H"

PCB_LIST* System::allThreads = new PCB_LIST();
SEM_LIST* System::allSem = new SEM_LIST();
Thread* System::idleThread = new Idle();
IVTEntry* System::interruptEntries[256]={0};
int System::globallyBlockedSignals[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int System::tstBrojac = 0;

volatile unsigned int System::runningTimeCntr=2;
volatile unsigned int System::lockFlag=1;
volatile unsigned int System::context_switch_on_demand=0;
volatile int System::numberOfUserThreads=0;

volatile int System::zaTestiranje=0;

pInterrupt oldTimer;
unsigned stara=0x8;
unsigned nova=0x60;

//pomocne promenljive za prekid tajmera
unsigned tsp;
unsigned tss;
unsigned tbp;
unsigned tid;

unsigned brojac = 0;

void tick();

// nova prekidna rutina tajmera
void interrupt timer(...){

	//cout<<"Usao sam u timer"<<endl;

	if (!System::context_switch_on_demand){
		tick();
		System::runningTimeCntr--;
		System::allSem->updateTimeBlockedThreads();
	}

	if (!System::context_switch_on_demand && PCB::runnig->quantum==0) return;

	if (System::runningTimeCntr == 0 || System::context_switch_on_demand) {

		if(System::lockFlag){
			System::context_switch_on_demand=0;
			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::runnig->sp = tsp;
			PCB::runnig->ss=tss;
			PCB::runnig->bp=tbp;

			if(PCB::runnig->state==PCB::READY && PCB::runnig!= System::idleThread->myPCB) Scheduler::put((PCB*)PCB::runnig);
			PCB::runnig=Scheduler::get();

			while(PCB::runnig->getState()==PCB::FINISHED){
				PCB::runnig=Scheduler::get();
			}

			if(PCB::runnig==0){
				if(System::numberOfUserThreads>0) {
					PCB::runnig = System::idleThread->myPCB;
				}
				else{
					if(PCB::main->getState()==PCB::BLOCKED) {
						PCB::runnig = System::idleThread->myPCB;
					}
					PCB::runnig = PCB::main;
				}
			}

			tsp = PCB::runnig->sp;
			tss = PCB::runnig->ss;
			tbp = PCB::runnig->bp;

			tid = PCB::runnig->id;
			System::runningTimeCntr = PCB::runnig->quantum;


			asm {
				// restaurira sp
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}


			if(PCB::runnig!=0 && PCB::runnig!=PCB::main){
				if(PCB::runnig->signalsWaitngForExecution->getNumOfSignalsWaitForExec()>0){
					//cout<<"PCB:"<< PCB::runnig->id<<"| br signala na cekanju:"<<PCB::runnig->signalsWaitngForExecution->getNumOfSignalsWaitForExec()<<endl;
					PCB::runnig->signalsWaitngForExecution->updateList();
				}
			}
		}
		else{
			System::context_switch_on_demand=1;
		}
	}

	// poziv stare prekidne rutine
	// koja se nalazila na 08h, a sad je na 60h;
	// poziva se samo kada nije zahtevana promena konteksta
	// tako da se stara rutina poziva
	// samo kada je stvarno doslo do prekida
	if(!System::context_switch_on_demand) asm int 60h;

}

// postavlja novu prekidnu rutinu
void System::inic(){
	asm cli
	//cuvam staru prekidnu rutinu
	oldTimer=getvect(stara);
	//na 08h stavljam novu
	setvect(stara,timer);
	//staru premestam na 60h
	setvect(nova,oldTimer);

	PCB::main=new PCB();
	PCB::runnig=PCB::main;

	asm sti
}

// vraca staru prekidnu rutinu
void System::restore(){

	asm cli
	setvect(stara,oldTimer);

	delete System::allThreads;
	delete System::idleThread;
	delete System::allSem;

	delete PCB::main;
	//delete PCB::runnig;

	System::allSem=0;
	System::allThreads=0;

	asm sti
}

// sinhrona promena konteksta
void System::dispatch(){
	asm pushf;
	asm cli
	System::context_switch_on_demand = 1;
	timer();
	asm popf;
}
