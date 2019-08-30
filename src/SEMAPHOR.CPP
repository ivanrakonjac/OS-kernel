#include "SEMAPHOR.H"
#include "KRNELSEM.H"
#include "SYSTEM.H"

Semaphore::Semaphore(int init){
	lock
	myImpl = new KernelSem(init);
	unlock
}

Semaphore::~Semaphore(){
	lock
	delete myImpl;
	unlock
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::val() const{
	lock
	int valuee=myImpl->val();
	unlock
	return valuee;
}
