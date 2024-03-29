
.NOTPARALLEL:

# Toolchain setup.
TOOLS = ../../../tools
GEN_TEST_PROGS = ../../../test-progs

# Platform-specific executables. Only work with O64.
ifeq ($(COMPILER), O64)
EXECUTABLES = convolution_3x3 convolution_5x5 convolution_7x7 greyscale median mandel mandel_multi
endif

# Platform-specific code to link.
OBJECTS = platform.a

# Stuff to clean in addition to the usual.
CLEAN = simtestall

# Include generic test program makefile.
include $(GEN_TEST_PROGS)/include.makefile

# Simulator command line.
SIM_PREFIX = ADAPTOR_CONFIG=cfg/plugin.conf LD_LIBRARY_PATH=$(TOOLS)/simrvex/lib64/sim200
SIM_PREFIX_NOFB = ADAPTOR_CONFIG=cfg/plugin-nofb.conf LD_LIBRARY_PATH=$(TOOLS)/simrvex/lib64/sim200
SIM_EXEC = $(TOOLS)/simrvex/bin/simrvex
SIM = $(SIM_PREFIX) $(SIM_EXEC)
SIM_NOFB = $(SIM_PREFIX_NOFB) $(SIM_EXEC)
SIMFLAGS += -e

# How to create the platform.a library
platform.a: platform.o simrvex_stubs.o
	$(AR) r $@ $^

.PHONY: help
help:
	@echo ""
	@echo " This platform allows you to easily run test programs on the simrvex simulator."
	@echo ""
	@echo ""
	@echo " Make targets:"
	@echo ""
	@echo "  make <prog>          : just compiles the specified program."
	@echo ""
	@echo "  make sim-<prog>      : runs prog on the simulator"
	@echo ""
	@echo "  make simtest-<prog>  : runs prog on the simulator and checks return code."
	@echo ""
	@echo "  make simtestall      : calls simtest-<prog> for all executables."
	@echo "                         will output a list of success/failures to file"
	@echo "                         \'simtestall\'"
	@echo ""
	@echo "  make clean           : cleans intermediate files."
	@echo ""
	$(MAKE) --no-print-directory list-executables
	@echo ""

# How to run simrvex
sim-%: %.elf
	${SIM} $< $(SIMFLAGS)

.PHONY: simtestall
simtestall: simtestall-clean $(POWERSTONE:%=simtest-%)

.PHONY: simtestall-clean
simtestall-clean:
	echo -n "" > simtestall

simtest-% : %.elf
	@${SIM} -e -t1,0 --batch $^|tail -n 40 | grep "RET: 0";\
	if [ $$? != 0 ] ; then \
	echo "Program returned unexpected exit code" ; echo $* >> simtestall; echo -e "\t\tError!" >> simtestall; \
	else \
	echo "Program Finished Successfully" ; echo $* >> simtestall; echo -e "\t\tOK" >> simtestall;  \
	fi

.PRECIOUS: simtrace-%
trace-% : %.elf
	${SIM} -e -t1,0 $< > sim$@

.PHONY: conformance
conformance: conformance-H8 conformance-O8 conformance-O4 conformance-O2

.PHONY: conformance-H8
conformance-H8:
	@$(CONFORM) "Compile all with HP 8-issue + vexparse -O2"                  "$(MAKE) -ks clean conformance-compile-all COMPILER=HP ISSUE_WIDTH=8 DYNAMIC=true-O2"
	@$(CONFORM) "Simulate benchmarks compiled with HP 8-issue + vexparse -O2" "$(MAKE) -ks conformance-run-all           COMPILER=HP ISSUE_WIDTH=8 DYNAMIC=true-O2"

.PHONY: conformance-O8
conformance-O8:
	@$(CONFORM) "Compile all with Open64 8-issue + vexparse -O2"                  "$(MAKE) -ks clean conformance-compile-all COMPILER=O64 ISSUE_WIDTH=8 DYNAMIC=true-O2"
	@$(CONFORM) "Simulate benchmarks compiled with Open64 8-issue + vexparse -O2" "$(MAKE) -ks conformance-run-all           COMPILER=O64 ISSUE_WIDTH=8 DYNAMIC=true-O2"

.PHONY: conformance-O4
conformance-O4:
	@$(CONFORM) "Compile all with Open64 4-issue + vexparse -O2"                  "$(MAKE) -ks clean conformance-compile-all COMPILER=O64 ISSUE_WIDTH=4 DYNAMIC=true-O2"
	@$(CONFORM) "Simulate benchmarks compiled with Open64 4-issue + vexparse -O2" "$(MAKE) -ks conformance-run-all           COMPILER=O64 ISSUE_WIDTH=4 DYNAMIC=true-O2"

.PHONY: conformance-O2
conformance-O2:
	@$(CONFORM) "Compile all with Open64 2-issue + vexparse -O2"                  "$(MAKE) -ks clean conformance-compile-all COMPILER=O64 ISSUE_WIDTH=2 DYNAMIC=true-O2"
	@$(CONFORM) "Simulate benchmarks compiled with Open64 2-issue + vexparse -O2" "$(MAKE) -ks conformance-run-all           COMPILER=O64 ISSUE_WIDTH=2 DYNAMIC=true-O2"

.PHONY: conformance-run-all
conformance-run-all: $(patsubst %,conformance-run-%,$(BENCH_EXECUTABLES))

.PHONY: conformance-run-%
conformance-run-%: %.elf
	@$(CONFORM) $* "$(SIM_NOFB) -e --run-cycles 50000000 -t1,0 --batch $^ | tail -n 40 | grep \"RET: 0\""
