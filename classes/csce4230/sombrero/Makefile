all: sombrero

%: %.c
	cc -lglut -o $@ $< -g -Wall -ansi -pedantic

clean:
	rm -f *.o pend

.PHONY: clean all
