//usermain.cpp

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "PCB.H"
#include "PCBLIST.H"
#include "IDLE.H"
#include "SYSTEM.H"
#include "SEMAPHOR.H"
#include <stdlib.h>


const int n = 10;
int count = 100;
Semaphore mutex(1);

int mutex_glb = 1;

void mutex_glb_wait(){
 sloop:asm{
	mov ax, 0
	xchg ax, mutex_glb
}
 if(_AX ==0){
	dispatch();
	asm jmp sloop;
 }
}


#define intLock mutex_glb_wait();
#define intUnlock mutex_glb = 1;


class BoundedBuffer {
public:

	BoundedBuffer (unsigned size);
	virtual ~BoundedBuffer ();
	int append (char);
	char take ();
	int  fullCount(){return itemAvailable.val();};    // potrebno consumeru

private:
	unsigned Size;
	Semaphore mutexa, mutext;
	Semaphore spaceAvailable, itemAvailable;
	char* buffer;
	int head, tail;

};

BoundedBuffer::BoundedBuffer (unsigned size) : Size(size),
	mutexa(1), mutext(1), spaceAvailable(size), itemAvailable(0),
	head(0), tail(0) {
		buffer = new char[size];
		if (!buffer) exit(1);
	}

BoundedBuffer::~BoundedBuffer(){
	intLock
	delete [] buffer;
	intUnlock
}

int BoundedBuffer::append (char d) {
	lock
	cout<<endl<<"-----------------------------"<<endl;
	cout<<"append() "<<d<<endl;
	unlock
	spaceAvailable.wait(0);
	mutexa.wait(0);
		buffer[tail] = d;
		tail = (tail+1)%Size;
	mutexa.signal();
	itemAvailable.signal();
	lock
	cout<<"itemAvailable: "<<itemAvailable.val()<<endl;
	cout<<"spaceAvailable: "<<spaceAvailable.val()<<endl;
	cout<<"-----------------------------"<<endl;
	unlock
	return 0;
}

char BoundedBuffer::take () {
	lock
	cout<<endl<<"-----------------------------"<<endl;
	unlock
	itemAvailable.wait(0);
	mutext.wait(0);
		char d = buffer[head];
		head = (head+1)%Size;
		lock
		cout<<"take() "<<d<<endl;
		unlock
	mutext.signal();
	spaceAvailable.signal();
	lock
	cout<<"itemAvailable: "<<itemAvailable.val()<<endl;
	cout<<"spaceAvailable: "<<spaceAvailable.val()<<endl;
	cout<<"-----------------------------"<<endl;
	unlock
	return d;
}

BoundedBuffer buffer(n/2);

void tick(){};

class Producer: public Thread
{
public:
	Producer(): Thread() {};
	~Producer()
	{
		waitToComplete();
	}

protected:
	void run()
	{
			buffer.append('c');
			buffer.append('d');
			buffer.append('e');
			buffer.append('f');
			buffer.append('g');
	}
};

class Consumer: public Thread
{
public:
	Consumer(): Thread() {};
	~Consumer()
	{
		waitToComplete();
	}

protected:
	void run()
	{
			buffer.take();
			buffer.take();
			buffer.take();
			buffer.take();
			buffer.take();
			buffer.take();
	}
};

int userMain(int argc, char *argv[]) {

	cout<<"USer main"<<endl;

	Producer p;
	p.start();
	Consumer c;
	c.start();

	cout<<"Srecan kraj!"<<endl;

	return 0;
}
