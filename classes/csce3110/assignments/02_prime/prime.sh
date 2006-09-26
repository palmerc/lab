#!/bin/sh

x=1
while [ 1 ]; do
   ./prime $x
   ((x++))
   sleep 1
done

