# UNIX Makefile

CXX	= g++
LD	= g++
CXXFLAGS = -g -pedantic

TestLogicFunction: LogicFunctionADT.o LogicFunctionTable.o LogicProcessor.o LogicFunction.o Tester.o
	$(LD) -o main $^

clean:
	$(RM) *.o main
