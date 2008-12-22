# UNIX Makefile

CXX	= g++
LD	= g++
CXXFLAGS = -g

testlogicfunction: logicfunction.o testlogicfunction.o
	$(LD) -o testlogicfunction $^

clean:
	$(RM) *.o testlogicfunction
