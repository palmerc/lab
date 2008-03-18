CC =c++
DIST =ana_indirect_cd

WAR =-Wall -Wcast-align -Wcast-qual -Werror

OPT =-g $(WAR) -O6
OPT =-O6 -funroll-loops -finline-functions -static

OBJECTS =\
	bb.o\
	du_chain.o\
	icd.o\
	main.o

all:	$(DIST)

$(DIST): $(OBJECTS)
	$(CC) $(OPT) -o $(DIST)  $(OBJECTS)

$(OBJECTS):
	$(CC) $(OPT) -c $<

clean:
	-rm -f *~ $(DIST) *.o *.bak 

c:
	-rm -f $(DIST) $(OBJECTS)

rm:
	-rm -f *.o *~ *.bak

# OBJECTS
bb.o		: bb.cc bb.h main.h
du_chain.o	: du_chain.cc du_chain.h bb.h main.h
icd.o		: icd.cc icd.h bb.h du_chain.h main.h
main.o		: main.cc bb.h icd.h main.h du_chain.h
