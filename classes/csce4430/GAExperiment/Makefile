# Joseph Parrish
# Program 3
#   written on a 98-column terminal,
#   so sorry if it looks poorly formatted.

CC = g++
RM = rm -f

CFLAGS = -g -Wall -ansi -pedantic
LDFLAGS = 

OBJS = tour.o driver.o

all: driver
.PHONY: all

driver: $(OBJS) 
	$(CC) $(OBJS) -o gen

tags: $(SRCS)
	$(CTAGS) $(SRCS)
	
clean:
	$(RM) gen $(OBJS) 

# DO NOT DELETE

#driver.o: sorted_list.h tour.h
