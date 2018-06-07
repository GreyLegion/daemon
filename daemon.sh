#! /bin/sh

. /home/grey/daemon/init.d

prog="daemon"
app="/home/grey/daemon/$prog"
proguser=root

start(){
	[ -x $exec ] || exit 5
	
	echo -n $"Starting $prog: "
	 $app -d
	RETVAL=$?
	[ $RETVAL -eq 0 ]
	echo
    return $RETVAL
}

stop(){
	echo -n $"Stopping $prog: "
	pkill $prog -INT
}

restart(){
	stop
    start
}

help(){
	$app -h
}

h(){
	help
}

case "$1" in
	start)
	$1
	;;
	stop)
	$1
	;;
	help)
	$1
	;;
	h)
	$1
	;;
	
	*)
	echo $"Usage: $0 {start|stop|restart|help(h)}"
	exit 2
esac

exit $?
