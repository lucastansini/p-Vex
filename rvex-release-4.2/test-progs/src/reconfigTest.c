#include "reconfigFunctionExample.h"




int newConfig = 0x8888;



int main(void) {

	if(reconfigure(newConfig)){
		rvex_succeed("Reconfiguration made with success.");	
	}



}

