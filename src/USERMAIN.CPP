//usermain.cpp

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "PCB.H"
#include "PCBLIST.H"
#include "IDLE.H"
#include "SYSTEM.H"
#include "SEMAPHOR.H"

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

class TestThread2 : public Thread {
public:
	TestThread2(Semaphore* s): Thread(4096, 20){
		sem = s;
	};
	~TestThread2() {}
protected:
	Semaphore* sem;
	void run();
};

void TestThread2::run() {
	for (int i =0; i < 30; ++i) {
		lock
		cout<<"funkcija_"<<PCB::runnig->getID()<<" "<<i<<endl;
		unlock

		if(i==5) {
			sem->wait(3);
		}
		//if(i==20) sem->signal(0);

		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

class TestThread3 : public Thread {
public:
	TestThread3(Semaphore* s): Thread(4096, 20){
		sem = s;
	};
	~TestThread3() {}
protected:
	Semaphore* sem;
	void run();
};

void TestThread3::run() {
	for (int i =0; i < 30; ++i) {
		lock
		cout<<"funkcija_"<<PCB::runnig->getID()<<" "<<i<<endl;
		unlock

		if(i==8) {
			sem->wait(100);
		}
		//if(i==20) sem->signal(0);

		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

int userMain(int argc, char *argv[]) {
	cout<<"USer main"<<endl;

	Semaphore* sem1 = new Semaphore();

	TestThread2* tt = new TestThread2(sem1);
	tt->start();

	TestThread2* tt2 = new TestThread2(sem1);
	tt2->start();

	TestThread3* tt3 = new TestThread3(sem1);
	tt3->start();

	PCB* main = new PCB();
	PCB::runnig = main;



	for (int i = 0; i < 15; ++i) {
		lock
		cout<<"main "<<i<<endl;
		unlock

		if(i==10) sem1->signal(0);

		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}
	cout<<"Srecan kraj!"<<endl;

	return 0;
}
