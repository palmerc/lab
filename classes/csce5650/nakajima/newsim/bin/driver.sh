#!/bin/bash

# This program will keep changing the HOST variable so the simulator
# will run all tests on a single machine. You should be warned that it
# took 48 days to run these tests in this fashion.
if [ ! -L $HOME/run ]; then
	echo "Creating $HOME/run link."
	ln -s $PWD/run $HOME/run
fi

if [ ! -d $HOME/result ]; then
	echo "Creating $HOME/result directory."
	mkdir $HOME/result
fi

if [ ! -d $HOME/proc ]; then
	echo "Creating $HOME/proc directory."
	mkdir $HOME/proc
fi
	
declare -a HOSTS
HOSTS=( `cat run | grep 'HOST:' | tr -d 'HOST:' | tr '\n' ' '` )
HOSTS_COUNT=${#HOSTS[*]}

for (( i=0; i < HOSTS_COUNT; i+=1 )); do
	echo "TEST: $i"
	HOST=${HOSTS[i]}; export HOST
	./run_newsim.pl
done

echo 'Tests have finished.' | $HOME/mailx user@example.com
