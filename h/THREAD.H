
#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms

typedef void (*SignalHandler)();
typedef unsigned SignalId;

typedef int ID;

class PCB; // Kernel's implementation of a user's thread
class Idle;
class PCB_LIST;

class Thread {
	public:

		void start(); 
		void waitToComplete();
	 	virtual ~Thread();

	 	ID getId();
	 	static ID getRunningId();
	 	static Thread * getThreadById(ID id);

	 	static int threadNum;


	 	void signal(SignalId signal);
	 	//void signal(SignalHandler sh);

	 	void registerHandler(SignalId signal, SignalHandler handler);
		void unregisterAllHandlers(SignalId id);

		void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

		void blockSignal(SignalId signal);
		void unblockSignal(SignalId signal);

		static void blockSignalGlobally(SignalId signal);
		static void unblockSignalGlobally(SignalId signal);


	protected:

		Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

		void setMyPCB(PCB* pcb){
			this->myPCB=pcb;
		}

	 	friend class PCB;
	 	friend class Idle;
	 	friend void interrupt timer(...);
	 	virtual void run() {};
	
	private:

	 	PCB* myPCB;
};

void dispatch ();

#endif
