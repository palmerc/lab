all:	lab1                 # _All_ we want to compile is: lab1

lab1:	lab1.o list.o        # The executable lab1 requires these two .o files
	g++ -g -o lab1 lab1.o list.o 
		              #...and the next line is the command to create it
lab1.o:	lab1.cc list.h
	g++ -g -c lab1.cc

list.o:	list.cc list.h
	g++ -g -c list.cc
