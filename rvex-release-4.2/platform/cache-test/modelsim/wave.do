onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /testbench/rvex_inst/rv2sim
add wave -noupdate /testbench/rvex_inst/cfg2any_configWord
add wave -noupdate /testbench/rv2rctrl_done
add wave -noupdate /testbench/idleContexts
add wave -noupdate /testbench/rvex_inst/rv2rctrl_idle
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {58215834 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 326
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {58194197 ps} {58309141 ps}
