CC	=c++
DIST	=ana_dataflow

WAR	=-Wall -Wcast-align -Wcast-qual -Werror

OPT	=-g $(WAR) -O6
OPT	=-O6 -funroll-loops -finline-functions -static

OBJECTS	=\
	bb.o\
	def_use.o\
	live.o\
	reach.o\
	main.o

all:	$(DIST)

$(DIST): $(OBJECTS)
	$(CC) $(OPT) -o $(DIST) $(OBJECTS)

$(OBJECTS):
	$(CC) $(OPT) -c $<

clean:
	-rm -f $(DIST) $(OBJECTS) *.cc~ *.h~ *.bak

c:
	-rm -f $(DIST) $(OBJECTS)

rm:
	-rm -f $(OBJECTS) *~ *.bak

# OBJECTS
bb.o		: bb.cc bb.h main.h 
def_use.o	: def_use.cc def_use.h main.h
live.o		: live.cc live.h bb.h def_use.h main.h
main.o		: main.cc main.h bb.h def_use.h live.h reach.h
reach.o		: reach.cc reach.h bb.h def_use.h main.h
