#!/bin/bash

# A quick hack I did to list off all the different tests the simulator would
# attempt.
# - Cameron Palmer, May 2008
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
	./list_sims.pl
done

