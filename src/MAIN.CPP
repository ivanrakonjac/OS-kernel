#include "System.h"
#include <DOS.H>
#include <stdlib.h>
#include <malloc.h>
#include "pcb.h"
#include "KRNELEV.H"

int userMain(int argc, char *argv[]);

int main(int argc, char *argv[]) {

	System::inic();

	int ret = userMain(argc, argv);

	System::restore();

	return ret;
}
