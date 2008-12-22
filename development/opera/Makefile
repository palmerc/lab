# UNIX Makefile

CXX	= g++
LD	= g++
CXXFLAGS = -g -pedantic

TestLogicFunction: LogicFunctionT.o LogicProcessor.o LogicFunctionList.o LogicFunction.o TestLogicFunction.o
	$(LD) -o main $^

clean:
	$(RM) *.o TestLogicFunction
