# UNIX Makefile

CXX	= g++
LD	= g++
CXXFLAGS = -g -pedantic

TestLogicFunction: LogicFunctionADT.o LogicProcessor.o LogicFunctionUser.o LogicFunctionSymmetry.o LogicFunction.o Tester.o
	$(LD) -o main $^

clean:
	$(RM) *.o main
