#  Time-stamp: <04/02/07 14:42:31 nakajima>

CC =c++
SIM =newsim
FLAG =-g -pg
WAR =-Wall -Wcast-align -Wcast-qual -Werror

OPT =-O1 $(FLAG) $(WAR)
OPT =-O6 -march=pentiumpro -funroll-loops -finline-functions -static

OBJECTS =bb.o data.o fork.o icd.o loop.o main.o\
	 pred.o print.o re.o sim.o trace.o

all:	$(SIM)

$(SIM): $(OBJECTS)
	$(CC) $(OPT) -o $(SIM) $(OBJECTS)
	@strip $(SIM)

$(OBJECTS):
	$(CC) $(OPT) $(DEBUG) -c  $<

clean:
	rm -f *.cc~ *.h~ *.bak $(SIM) *.o gmon.out ../input/*/gmon.out

c:
	rm -f $(SIM) *.o a.out

rm:
	rm -f *.cc~ *.h~ *.bak ../input/*/gmon.out

bak:
	rm -f *.bak gmon.out ../input/*/gmon.out
	/home/nakajima/bin/nsim_backup

# OBJECTS
bb.o:		bb.cc\
		bb.h pred.h print.h re.h sim.h
data.o:		data.cc bb.h main.h trace.h\
		data.h print.h re.h sim.h
fork.o:		fork.cc bb.h re.h trace.h\
		fork.h data.h icd.h loop.h pred.h print.h sim.h
icd.o:		icd.cc bb.h\
		icd.h fork.h main.h print.h sim.h
loop.o:		loop.cc trace.h\
		loop.h bb.h fork.h main.h pred.h print.h re.h sim.h
main.o:		main.cc main.h bb.h data.h fork.h icd.h loop.h\
		pred.h print.h re.h sim.h trace.h
pred.o:		pred.cc trace.h\
		pred.h bb.h print.h re.h sim.h
print.o:	print.cc trace.h\
		print.h data.h loop.h main.h pred.h re.h sim.h
re.o:		re.cc main.h trace.h\
		re.h bb.h data.h fork.h icd.h loop.h pred.h print.h sim.h
sim.o:		sim.cc sim.h print.h trace.h
trace.o:	trace.cc trace.h
