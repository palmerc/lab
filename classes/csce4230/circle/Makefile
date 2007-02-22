all: circ

%: %.c
	cc -lglut -o $@ $< -g -Wall -ansi -pedantic

clean:
	rm -f *.o circ

.PHONY: clean all
