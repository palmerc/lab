#!/bin/sh

a=0
while [ 1 ]; do
   ((a++))
   ./fib1 $a
done
