//usermain.cpp

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "PCB.H"
#include "PCBLIST.H"
#include "IDLE.H"
#include "SYSTEM.H"
#include "SEMAPHOR.H"
#include "EVENT.H"
#include <stdlib.h>

PREPAREENTRY(9,1);

const int n = 16;

void tick(){}


class TestThread : public Thread
{
public:

	TestThread(): Thread(4096,0) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	Event e(9);

	while(1){
		lock
		cout<<"Test thread"<<endl;
		unlock
		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);


		e.wait();
	}

}

int userMain(int argc, char *argv[]) {

	TestThread t;
	t.start();

	for (int i = 0; i < 30; ++i) {
			lock
			cout<<"main "<<i<<endl;
			unlock

			for (int j = 0; j< 30000; ++j)
				for (int k = 0; k < 30000; ++k);
		}

	cout<<"Srecan kraj! "<<System::tstBrojac<<endl;

	return 0;
}
