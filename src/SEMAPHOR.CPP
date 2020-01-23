#include "SEMAPHOR.H"
#include "KRNELSEM.H"
#include "SYSTEM.H"

Semaphore::Semaphore(int init){
	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore(){
	delete myImpl;
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::val() const{
	int valuee=myImpl->val();
	return valuee;
}
