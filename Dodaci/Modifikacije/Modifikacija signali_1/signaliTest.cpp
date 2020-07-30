#include "thread.h"
#include "semaphor.h"
#include "event.h"
#include "System.h"
#include "IVTENTRY.H"
#include "PCB.H"

/*
	Test signala
*/

#include <iostream.h>
#include <stdlib.h>

PREPAREENTRY(9,1);

void tick() {}

class TestNit : public Thread{

public:
	TestNit(char znak) : Thread(), znak(znak){}
	virtual ~TestNit(){waitToComplete();};

	void run(){
		for (int i = 0; i < 10; ++i)
		{
			cout<<znak;
		}
		cout<<endl;
	}

private:
	char znak;
};

void signal1(){
	cout<<"Nit id:"<<PCB::runnig->getID()<<" signal 1"<<endl;
}

void signal5(){
	cout<<"Nit id:"<<PCB::runnig->getID()<<" signal 5"<<endl;
}
void signal52(){
	cout<<"Nit id:"<<PCB::runnig->getID()<<" signal 5.2"<<endl;
}

void signal7(){
	cout<<"Nit id:"<<PCB::runnig->getID()<<" signal 7"<<endl;
}

void signal8(){
	cout<<"Nit id:"<<PCB::runnig->getID()<<" signal 8"<<endl;
}

SignalHandler nizSignala[5] = {signal1,signal5,signal52,signal7,signal8};

int userMain(int argc, char* argv[]){

	TestNit* tn = new TestNit('a');
	TestNit* tn2 = new TestNit('b');

	tn->registerHandler(5,nizSignala,22);
	//tn2->registerHandler(7,nizSignala,5);

	tn->start();
	//tn2->start();

	tn->signal(5);
	//tn2->signal(7);

	delete tn;
	cout<<"userMain finished"<<endl;
	return 0;
}
