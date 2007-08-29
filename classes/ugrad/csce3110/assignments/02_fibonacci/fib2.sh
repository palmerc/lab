#!/bin/sh

a=0
while [ 1 ]; do
   ((a++))
   ./fib2 $a
done
