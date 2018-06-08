#!/bin/bash

NUM_FAULTS=100000
dateBegin="$(date)"

for ((i=0; i<$NUM_FAULTS; ++i )) ; 
do
  faults=$(($faults+1))
  (make vsim-run-fault-inj) #| grep "Warning" | egrep -v "vsim|vlib|ALTERA|faultTime"
  echo "------------------------------------"
  echo "Fault batch: "$i
  echo "Date begin: "$dateBegin
  dateEnd="$(date)"
  echo "Date end: "$dateEnd
  echo "------------------------------------"
done



