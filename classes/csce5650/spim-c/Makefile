CXXFLAGS=-g

all: spimcc

spimcc: lexer.o spimcc.o
	cc $(LDFLAGS) lexer.o spimcc.o -Wall -ansi -pedantic --std=c99 -o $@

lexer.o: lexer.c lexer.h global.h
parser.o: parser.c parser.h lexer.h
spimcc.o: spimcc.c lexer.h

clean:
	-rm -f *.o spimcc

.PHONY: clean all
