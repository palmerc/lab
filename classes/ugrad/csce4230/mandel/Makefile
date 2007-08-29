all: mandel

%: %.c
	cc -lglut -o $@ $< -g -Wall -ansi -pedantic

clean:
	rm -f *.o main

.PHONY: clean all
