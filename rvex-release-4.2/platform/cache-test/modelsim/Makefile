
RVLIB = ../../../lib/rvex
VHDL_LIBS = unisim common utils bus core cache work

include ../../share/modelsim.makefile

compile-work.do:
	@echo "vlib work" > $@
	@echo "vcom -quiet -93 -work work \"../design/testbench.vhd\"" >> $@

#New modifications are below this line
.PHONY: vsimrun
vsimrun: compile.do
	$(VSIM) $(VSIMFLAGS) < runsim.do

.PHONY: vsimruncyclecount
vsimruncyclecount: compilecycle.do
		$(VSIM) $(VSIMFLAGS) < runsimcycle.do

