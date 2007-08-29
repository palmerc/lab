all: kmb

%: %.cpp
	c++ -lboost_regex -o $@ $< -g -Wall -ansi -pedantic

clean:
	rm -f *.o kmb

.PHONY: clean all
