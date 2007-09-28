#!/bin/bash

DEBUG="0"
rounds="0"

# Conway's Game of Life
# An example of cellular automata

# Rules to Conway's Game of Life
# Survivals - Every counter with two or three neighbors survives for the next 
# generation.
#
# Deaths - Every counter with four or more neighbors dies from overpopulation. 
# Every counter with one neighbor or none dies from isolation.
#
# Births - Each empty cell adjacent to exactly three neighbors, no more, no 
# fewer, is a birth cell. A counter is placed on it at the next move.

declare -a gameBoard
declare -a tempBoard

# Calculate a round 
function Round {
	local i=0
	for (( i=0; i < gameBoardSize; i+=1 )); do
		# Calculating my position in 2d grid arithmetic
		local row=$(( i / SIZE ))
		local col=$(( i % SIZE ))
	#	echo "Row=${row} Col=${col}"

		# Calculating neighbors position in single array arithmetic
		local north=$(( i - SIZE ))
		local northeast=$(( i - SIZE + 1 ))
		local northwest=$(( i - SIZE - 1 ))
		local south=$(( i + SIZE ))
		local southeast=$(( i + SIZE + 1 ))
		local southwest=$(( i + SIZE - 1 ))
		local east=$(( i + 1 ))
		local west=$(( i - 1 ))
#		echo "$north $northeast $east $southeast $south $southwest $west $northwest"
		
		local counter=0
		if [ ${north} -ge 0 ]; then
			if [ "${gameBoard[north]}" = "." ]; then
				let "counter += 1"
			fi
		fi
		if [ $(( northeast%SIZE )) -eq $(( col+1 )) -a ${northeast} -ge 0 ]; then
			if [ "${gameBoard[northeast]}" = "." ]; then
				let "counter += 1"
			fi
		fi
		if [ $(( northwest%SIZE )) -eq $(( col-1 )) -a ${northwest} -ge 0 ]; then
			if [ "${gameBoard[northwest]}" = "." ]; then
				let "counter += 1"
			fi
		fi
		if [ ${south} -le $gameBoardSize ]; then
			if [ "${gameBoard[south]}" = "." ]; then
				counter=$(( counter + 1 ))
			fi
		fi
		if [ $(( southeast%SIZE)) -eq $(( col+1 )) -a ${southeast} -le $gameBoardSize ]; then
			if [ "${gameBoard[southeast]}" = "." ]; then
				counter=$(( counter + 1 ))
			fi
		fi
		if [ $(( southwest%SIZE)) -eq $(( col-1 )) -a ${southwest} -le $gameBoardSize ]; then
			if [ "${gameBoard[southwest]}" = "." ]; then
				counter=$(( counter + 1 ))
			fi
		fi
		if [ $east -le $gameBoardSize -a $(( east / SIZE )) -eq $row ]; then
			if [ "${gameBoard[east]}" = "." ]; then
				let "counter += 1"
			fi
		fi
		if [ $west -ge 0 -a $(( west / SIZE )) -eq $row ]; then
			if [ "${gameBoard[west]}" = "." ]; then
				counter=$(( counter + 1 ))
			fi
		fi
#		echo "Neighbors $counter"

		if [ "${gameBoard[i]}" = "." ]; then
			# Survives
			if [ $counter -eq 2 -o $counter -eq 3 ]; then
				tempBoard[i]="."
#				echo "Survives"
			fi
			# Dies
			if [ $counter -ge 4 -o $counter -le 1 ]; then
				tempBoard[i]="_"
#				echo "Dies"
			fi
		fi

		if [ "${gameBoard[i]}" = "_" ]; then
			# Born
			if [ $counter -eq 3 ]; then
				tempBoard[i]="."
#				echo "Born"
			else
				tempBoard[i]="_"
#				echo "No change"
			fi
		fi
	done
	gameBoard=( "${tempBoard[@]}" )
}

# Function - Display
# Parameters: none
# Returns: none
function Display { 
	clear
	echo "Round $round"

	local i=0
	local j=1
	# This for loop is wrapped in parens to make it speedy
	# if you switch from $(()) to backticks you can really slow the program down
	(for (( i=0; i < gameBoardSize; i+=1 )); do 
		echo -n "${gameBoard[i]}" 
		# Try enabling the sed _ to space bit here for slow times
		#echo -n "${gameBoard[i]}" | sed 's/_/ /g'
		if [ $j -lt $SIZE ]; then
			j=$((j + 1))
		else
			echo
			j=1
		fi
	done) | sed 's/_/ /g'
}

# Main


if [ $# -ne 1 ]
then
	echo "Usage: $0 input_file"
	exit 1
fi

inputFile=$1

# Read in the text file and convert it to array friendly input
NAME=( `cat $inputFile | grep 'NAME' | sed 's/NAME=\([A-Za-z0-9 ]*\)/\1/'` )
SIZE=( `cat $inputFile | grep 'SIZE' | sed 's/SIZE=\([0-9]*\)/\1/'` )
ROUNDS=( `cat $inputFile | grep 'ROUNDS' | sed 's/ROUNDS=\([0-9]*\)/\1/'` )
gameBoard=( `cat $inputFile | grep -v '[NAME|SIZE|ROUNDS]' | sed 's/#.*//' | tr -d "\n" | sed 's/\([_.]\)/\1 /g'` )
gameBoardSize=${#gameBoard[*]}

# Simple code to see what the array contains
if ((DEBUG)); then
	echo "NAME is $NAME"
	echo "SIZE is $SIZE"
	echo "ROUNDS is $ROUNDS"
	i="0"
	while [ $i -lt $gameBoardSize ]
	do
		# -n prevents echo from outputing a newline
		echo -n ${gameBoard[i]}
		i=$(( $i + 1 ))
	done
	echo
fi

# Function call to Display
round=0
while [ $round -lt $ROUNDS ]; do
	Display
	Round	
	let "round += 1"
done

exit
