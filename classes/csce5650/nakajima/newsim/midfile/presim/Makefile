# Makefile 03/06/23
#  predict
#

CC	=c++
SIM	=presim

WAR	=-Wall -Wcast-align -Wcast-qual -Werror

OPT	=-g $(WAR) -O6
OPT	=-O6 -march=pentiumpro -funroll-loops -finline-functions -static

OBJECTS =bb.o data.o pred.o main.o re.o trace.o

all:	$(SIM)

$(SIM): $(OBJECTS)
	$(CC) $(OPT) -o $(SIM) $(OBJECTS)

$(OBJECTS):
	$(CC) $(OPT) $(DEBUG) -c $<

clean:
	rm -f *.cc~ *.h~ *.bak $(SIM) $(OBJECTS)

c:
	rm -f $(SIM) $(OBJECTS)

rm:
	rm -f *.cc~ *.h~ *.bak

# OBJECTS
bb.o:		bb.cc\
		bb.h main.h
data.o:		data.cc data.h\
		bb.h main.h
main.o:		main.cc\
		main.h bb.h pred.h trace.h
pred.o:		pred.cc bb.h main.h trace.h\
		pred.h
re.o:		re.cc trace.h\
		re.h main.h
trace.o:	trace.cc trace.h
