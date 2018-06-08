PROJECT="rvex-unprotected"
SIMTOP="my_tb"

vsim-launch: 
	@vsim -quiet $(PROJECT).mpf

vsim-run: 
	@vsim -t 1ps -c -novopt $(SIMTOP) glbl <  ./runvsim.do

vsim-run-fault-inj: 
	@vsim -t 1ps -c -novopt $(SIMTOP) glbl <  ./fault_inj_script.do
