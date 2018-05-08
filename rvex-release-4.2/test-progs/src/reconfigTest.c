#include "rvex.h"
#include <stdio.h>

int main() {


	//CR_CRR = 0x8811;

	/*CR_CRR = 0x0211 enables three cores.
	  CR_CRR = 0x0011 enables two cores.
	  CR_CRR = 0x3210 enables all four cores with 2 lanes each.

	*/


	int i=0;
	int j=0;

	for(i; i<10; i++){
	  j++;
	}

	rvex_succeed("End of test.");

}
