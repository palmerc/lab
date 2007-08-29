all: cube

%: %.c
	cc -lglut -o $@ $< -g -Wall -ansi -pedantic

clean:
	rm -f *.o cube

.PHONY: clean all
