do signals_injfaults.do

set NUM_FAULTS 250
set control "report_control_error.txt"
set memory "memory_dump.txt"
set memoryGold "memory_dump_gold.txt"
set reportErrors "errors_report.txt"
set simFile "report_simulation.txt"
set reportSim "errors_simulation.txt"
set filenameID [ clock format [clock seconds] -format {%d_%h_%Y-%H_%M_%S} ]
set execStatus "report_$filenameID.txt"
set faults 0
set controlErrors 0
set dataErrors 0
set simErrors 0
set systemTime [clock seconds]
set dateBegin [clock format $systemTime -format {%T - %D}]
set signalPC "sim:/my_tb/DUT/rvex_1/mpc_r "

#Project Definitions
set faultDuration 20
set execTime 1750000
set signalsSize [array size signal]

proc -s {filename} {
  set rc [catch {file size $filename} size]
  return [expr {$rc == 0 && $size > 0}]
}

proc myRand { min max } { 
  set maxFactor [expr [expr $max + 1] - $min] 
  set value [expr int([expr rand() * 1000000000])] 
  set value [expr [expr $value % $maxFactor] + $min] 
  return $value 
}

proc dec2bin { int size } {
  set size [expr [format "%d" $size]-1]
  binary scan [binary format W $int] B* var
  set res [string range $var end-$size end]
}

for {set i 0} {$i<$NUM_FAULTS} {incr i} { 
  set faults [expr $faults+1]
  #############################################################################
  #injecting faults
  restart
  set faultTime [myRand 0 [expr $execTime-1]]
  set faultLocation [myRand 0 [expr $signalsSize-1]]

  # goes to injection time
  run $faultTime ps

  # fault injection
  set maxI [string length [string trim [examine -radix bin $signal($faultLocation)] St]]
  set forceVal 0
  set sig [format "%s" [examine -radix dec $signal($faultLocation)]]
  if {
      [string equal -nocase $sig "x"]
      || [string equal -nocase $sig "z"]
      || [string equal -nocase $sig "u"]
      || [string equal -nocase $sig "w"]
  } then {
    set forceVal [dec2bin 0 $maxI]
  } else {
    #set forceVal [dec2bin [expr ~([examine -radix dec $signal($faultLocation)])] $maxI]
    set faultBit [myRand 0 [expr $maxI-1]]
    set fSignal [string trim [examine -radix bin $signal($faultLocation)] St]
    for {set x 0} {$x<$maxI} {inc x} {
      if {$faultBit == $x} {
        if {$x == 0} {
					set forceVal [expr ![string index $fSignal $faultBit]]
				} else {
	        set forceVal ${forceVal}[expr ![string index $fSignal $faultBit]]
 				}
      } else {
				if {$x == 0} {
					set forceVal [string index $fSignal $x]
				} else {
	        set forceVal ${forceVal}[string index $fSignal $x]
				}
      }
    }
  }
 	set currentPC [examine -radix hex $signalPC]
  echo "Warning: PC-" $currentPC " Injecting " $forceVal " at " $signal($faultLocation) " -" $faultLocation " @ " $faultTime
  force -freeze $signal($faultLocation) $forceVal -cancel $faultDuration ns

  # runs application after the fault injection
  run -all
  mem save -o ./$memory -f mti -data binary -addr hex /my_tb/DUT/d_mem_1
  ##############################################################################
  if {[-s $control]} {
    puts "E: Control error"
    set controlErrors [expr $controlErrors+1]
    exec echo "CFE - PC:" $currentPC "- Injecting " $forceVal " at " $signal($faultLocation) " -" $faultLocation " @ " $faultTime >> $reportErrors
  } else {
    set status [catch {exec diff $memory $memoryGold} result]
    if {$status == 0} {
      puts "No error"
    } elseif {$status == 1} {
      if {[-s $simFile]} { #detects if the simulation crashed
      	puts "E: Data error"
	      set dataErrors [expr $dataErrors+1]
  	    exec echo "DE - PC:" $currentPC "- Injecting " $forceVal " at " $signal($faultLocation) " -" $faultLocation " @ " $faultTime >> $reportErrors  
      } else {
	      set simErrors [expr $simErrors+1]
      	exec echo "DE - PC:" $currentPC "- Injecting " $forceVal " at " $signal($faultLocation) " -" $faultLocation " @ " $faultTime >> $reportSim
      }
    } else {
      puts "Erro diff $status"
    }
  }
  exec echo "------------------------------------"  >  $execStatus
  exec echo "Faults injected: $faults"              >> $execStatus
  exec echo "Control errors:  $controlErrors"       >> $execStatus
  exec echo "Data errors:     $dataErrors"          >> $execStatus
  exec echo "Sim errors:      $simErrors"						>> $execStatus 
  set totalErrors [expr $simErrors+[expr $dataErrors+$controlErrors]]
  exec echo "Total errors:    $totalErrors"         >> $execStatus

  exec echo "Date begin: $dateBegin"                >> $execStatus
  set systemTime [clock seconds]
  set dateEnd [clock format $systemTime -format {%T - %D}]
  exec echo "Date end:   $dateEnd"                  >> $execStatus
  exec echo "------------------------------------"  >> $execStatus
}

echo "--Simulation is done--"
