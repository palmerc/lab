CC	=c++
DIST	=ana_loop_pc

WAR	=-Wall -Wcast-align -Wcast-qual -Werror

OPT	=-g $(WAR) -DDEBUG
OPT	=-O6 -funroll-loops -finline-functions -static

OBJECTS	=\
	bb.o\
	du_chain.o\
	inst.o\
	loop.o\
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
du_chain.o	: du_chain.cc bb.h du_chain.h
inst.o		: inst.cc inst.h main.h
loop.o		: loop.cc bb.h loop.h main.h du_chain.h
main.o		: main.cc main.h bb.h loop.h du_chain.h
