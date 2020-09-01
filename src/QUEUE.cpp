#include <stdlib.h>
#include "queue.h"
#include "SYSTEM.H"


Queue::Queue() {
	asm {pushf;cli;}
	first = 0;
	asm popf;
}

Queue::~Queue() {
	asm {pushf;cli;}
	Elem* temp;
	while( first != 0 )	{
		temp = first;
		first = first->next;
		delete temp;
	}
	asm popf;
}

PCB* Queue::get() {
	asm {pushf;cli;}
	if( first == 0 ) { asm popf; return 0; }
	Elem* temp = first;
	PCB *ret = temp->pcb;
	first = first->next;
	delete temp;
	asm popf;
	return ret;
}


void Queue::put(PCB* pcb) {
	asm {pushf;cli;}
	Elem **dp = &first;
	while( (*dp) != 0 )
	{
		if( (*dp)->pcb == pcb ) { asm popf; return; }
		dp = &((*dp)->next);
	}
	(*dp) = new Elem();
	(*dp)->next = 0;
	(*dp)->pcb = pcb;
	asm popf;
}
