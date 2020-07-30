#include "System.h"
#include <DOS.H>
#include <stdlib.h>
#include <malloc.h>
#include "pcb.h"
#include "KRNELEV.H"

int userMain(int argc, char *argv[]);

int counter = 0;

void* operator new(unsigned size) {
  void* ret = malloc(size);
  if(ret==0)
    exit(1);//Out of mem
  counter++;
  return ret;
}



void operator delete(void* loc) {
  if(loc != 0){
    counter--;
    free(loc);
  }
}


int main(int argc, char *argv[]) {

	/*lock
	cout<<"BR kernEv: "<<KernelEv::kerEvNum<<endl;
	unlock*/

	/*lock
	cout<<"Counter pre inica: "<<counter<<endl;
	cout<<"Br pcb:"<< PCB::pcbNum<<endl;
	unlock*/

	System::inic();

	/*lock
	cout<<"Counter posle inica: "<<counter<<endl;
	cout<<"Br pcb:"<< PCB::pcbNum<<endl;
	unlock*/

	int ret = userMain(argc, argv);

	/*lock
	cout<<"Counter posle maina: "<<counter<<endl;
	cout<<"Br pcb:"<< PCB::pcbNum<<endl;
	unlock*/

	System::restore();

	/*lock
	cout<<"BR kernEv: "<<KernelEv::kerEvNum<<endl;
	unlock*/

	/*lock
	cout<<"Counter posle restora: "<<counter<<endl;
	cout<<"Br pcb:"<< PCB::pcbNum<<endl;
	unlock*/

	return ret;
}
