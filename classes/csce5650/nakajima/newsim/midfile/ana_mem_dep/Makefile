# Makefile 03/06/22

CC =c++
DIST =mem_ana
FLAG =-g -pg
WAR =-Wall -Wcast-align -Wcast-qual -Werror

OPT =-O1 $(FLAG) $(WAR)
OPT =-O6 -funroll-loops -finline-functions -static

OBJECTS =data.o main.o

all:	$(DIST)

$(DIST): $(OBJECTS)
	$(CC) $(OPT) -o $(DIST) $(OBJECTS)

$(OBJECTS):
	$(CC) $(OPT) $(DEBUG) -c  $<

clean:
	rm -f *.cc~ *.h~ *.bak $(DIST) $(OBJECTS)

c:
	rm -f $(DIST) $(OBJECTS)

rm:
	rm -f *.cc~ *.h~ *.bak

# OBJECTS
data.o:		data.cc main.h data.h
main.o:		main.cc main.h data.h
