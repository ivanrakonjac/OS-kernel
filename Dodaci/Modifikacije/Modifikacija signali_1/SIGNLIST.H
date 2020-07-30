#ifndef _SIGNAL_HANDLER_LIST_H
#define _SIGNAL_HANDLER_LIST_H

typedef void (*SignalHandler)();

class SignalHandlerList{

	class SignalHandlerNode{
	public:
		SignalHandler signalHandler;
		SignalHandlerNode* n;

		SignalHandlerNode(SignalHandler sh, SignalHandlerNode* shnp=0){
			this->signalHandler = sh;
			this->n = shnp;
		}
	};

public:
	SignalHandlerList(){
		first = 0;
		last = 0;
	}
	~SignalHandlerList();

	void addToTheEnd(SignalHandler sh);
	void swap(SignalHandler sh1,SignalHandler sh2);
	void unregisterAllHandlers(unsigned id);

	void stampaj();

	SignalHandlerNode* first;
	SignalHandlerNode* last;
};

#endif
