

sim_exit::
	trap 1
	;;
	return $l0.0
	;;

/*
 * The parameter is a bitfield
 * TRACE_OFF             0
 * TRACE_USER_MODE       1
 * TRACE_SUPER_MODE      2
 * TRACE_ON  (TRACE_USER_MODE|TRACE_SUPER_MODE)
*/

sim_trace::
	trap 217
	;;
	return $l0.0
	;;

sim_write::
	/* Shuffle the arguments */
	mov	$r0.2 = 3
	;;
	trap 0x91
	;;
	return $l0.0
	;;

