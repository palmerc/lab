all: main

%: %.cpp
	c++ -o $@ $< -g -Wall -ansi -pedantic

clean:
	rm -f *.o main

.PHONY: clean all
