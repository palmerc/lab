#!/bin/bash

for word in bass crane motion palm plant tank
do
	./baseline.pl TWA.sensetagged/$word.train TWA.sensetagged/$word.test	
	./program3.pl TWA.sensetagged/$word.train TWA.sensetagged/$word.test	
	echo
done
