CC	=c++
DIST	=make_posdom
WAR	=-Wall -Wcast-align -Wcast-qual -Werror

#OPT	=-g $(WAR) -O6
OPT	=-O6 -funroll-loops -finline-functions -static

OBJECTS =\
	bb.o\
	main.o\
	posdom.o

all:	$(DIST)

$(DIST): $(OBJECTS)
	$(CC) $(OPT) -o $(DIST) $(OBJECTS)

$(OBJECTS):
	$(CC) $(OPT) -c $<

clean:
	-rm -f $(DIST) *.o *.cc~ *.h~ *.bak

c:
	-rm -f $(DIST) $(OBJECTS)

rm:
	-rm -f *.cc~ *.h~ *.o *.bak

#OBJECTS
bb.o    : bb.cc bb.h main.h
main.o	: main.cc bb.h main.h
posdom.o: posdom.cc posdom.h bb.h main.h
