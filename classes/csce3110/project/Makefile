all: kmb

%: %.cpp
	c++ -o $@ $< -g -Wall -ansi -pedantic

clean:
	rm -f *.o kmb

.PHONY: clean all
