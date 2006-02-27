#!/bin/sh

echo Number of arguments passed to the script $#
echo Argument 0: $0
count=1
for argument in $*
do
   echo Argument $count: $argument
   count=`expr $count + 1`
done
