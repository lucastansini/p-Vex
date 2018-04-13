
# Compile the VHDL files.
do compile.do

# Give simulate command.
vsim -t ps -novopt -L unisim work.testbench

onerror {resume}

#Change radix to Hexadecimal#
radix hex

# Supress spam.
set NumericStdNoWarnings 1
set StdArithNoWarnings 1

#Execute the wave.do file
do wave.do

#Start the simulation. Note that executing 'run -all' will execute the simulation infite times. Instead you can use 'run Xus'
run 100us

