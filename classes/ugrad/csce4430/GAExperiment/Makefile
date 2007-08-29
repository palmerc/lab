CC = g++
RM = rm -f

CFLAGS = -g -Wall -ansi -pedantic -c

OBJS = tour.o driver.o

all: driver

%.o: %.cpp
	$(CC) $(CFLAGS) $<

driver: $(OBJS) 
	$(CC) $(OBJS) -o gen
	$(RM) $(OBJS)

clean:
	$(RM) gen $(OBJS) 

.PHONY: all
# DO NOT DELETE
tour.cpp: tour.h
driver.cpp: sorted_list.h tour.h
driver.o: sorted_list.h tour.h
