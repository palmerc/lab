#!/bin/sh
for num in 0 1 2 3 4
do
	cd example$num
	make clean
	cd ..
done
