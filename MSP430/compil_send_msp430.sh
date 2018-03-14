#!/usr/bin/expect -f

#to use this bash you need to install expect package, and msp430 tools

logfile="msp430compilation.log"
msp430-gcc -mmcu=msp430g2253 $1 2> "${logfile}"
test=`cat $logfile`
if [ -z "$test" ] ; then
	echo "compilation succed"
	expect -c"
		set timeout -1
		spawn mspdebug rf2500
		expect \"(mspdebug)\"
		send -- \"prog a.out\r\"
		expect \"(mspdebug)\"
		send -- \"exit\r\"
		expect eof
		"
	expect eof
	fi
if [ "$test" != "" ] ; then
	echo "fatal error `cat $logfile`"
	fi
