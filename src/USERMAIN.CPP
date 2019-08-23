//usermain.cpp

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "PCB.H"
#include "PCBLIST.H"
#include "IDLE.H"
#include "SYSTEM.H"

class TestThread : public Thread {
public:
	TestThread(): Thread(4096, 20){};
	~TestThread() {}
protected:
	void run();
};



void TestThread::run() {
	for (int i =0; i < 30; ++i) {
		System::lockFlag=0;
		cout<<"funkcija_"<<PCB::runnig->getID()<<" "<<i<<endl;
		System::lockFlag=1;
		if(System::context_switch_on_demand)
			System::dispatch();
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

void tick() {
	//u javnom testu
}

int userMain(int argc, char *argv[]) {
	cout<<"USer main"<<endl;

	TestThread* tt = new TestThread();
	tt->start();

	int tst = tt->getId();

	System::allThreads->getPCBwithID(tst)->setState(PCB::BLOCKED);

	TestThread* tt2 = new TestThread();
	tt2->start();

	TestThread* tt3 = new TestThread();
	tt3->start();


	PCB* main = new PCB();
	PCB::runnig = main;

	for (int i = 0; i < 15; ++i) {
		lock
		cout<<"main "<<i<<endl;
		unlock

		if(i == 5) PCB::runnig->setState(PCB::BLOCKED);

		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}

	cout<<"tt state: "<<System::allThreads->getPCBwithID(tst)->getState()<<endl;
	cout<<"Srecan kraj!"<<endl;

	return 0;
}
