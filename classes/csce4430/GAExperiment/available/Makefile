# Joe Student
#   written on a 98-column terminal,
#   so sorry if it looks poorly formatted.

CC = g++
RM = rm -f

CFLAGS = -g -Wall 
CPPFLAGS = -g -Wall 

GEN_OBJS = driver.o tour.o
OBJS = test.o avltest.o $(GEN_OBJS)

gen: $(GEN_OBJS)
	$(CC) $(CFLAGS) -o gen $(GEN_OBJS)
	
test: test.o
	$(CC) $(CFLAGS) -o test test.o

avltest: avltest.o
	$(CC) $(CFLAGS) -o avltest avltest.o
	
clean:
	$(RM) test gen $(OBJS) core *.stackdump *~ *.exe

driver.o: sorted_list.h
test.o: sorted_list.h
avltest.o: sorted_list.h

