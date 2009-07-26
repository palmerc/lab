CXXFLAGS=-g

all: spimcc

spimcc: lexer.o parser.o spimcc.o
	cc $(CXXFLAGS) $(LDFLAGS) lexer.o parser.o spimcc.o -Wall -ansi -pedantic --std=c99 -o $@

lexer.o: lexer.c global.h lexer.h
parser.o: parser.c lexer.h
spimcc.o: spimcc.c global.h lexer.h

clean:
	-rm -f *.o spimcc

.PHONY: clean all
