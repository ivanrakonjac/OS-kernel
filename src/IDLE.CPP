#include "IDLE.H"
#include "PCB.H"
#include <iostream.h>

Idle::Idle():Thread(4096,1){
	cout<<"Idle::Idle()"<<endl;
	this->myPCB->setID(-100);
};

void Idle::run(){
	while(1){
	}
};
