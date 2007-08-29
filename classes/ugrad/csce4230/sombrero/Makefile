all: sombrero

%: %.c
	cc -lglut -o $@ $< -g -Wall -ansi -pedantic --std=c99

clean:
	rm -f *.o sombrero 

.PHONY: clean all
