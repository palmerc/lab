#!/bin/sh

i=0
while [ $i -lt 100 ]
do
   echo $i
   i=$(($i+1)) # The modern expr equivalent
done
