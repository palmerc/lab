all: main

%: %.cpp
	g++ -lboost_regex -o $@ $< -g -Wall -ansi -pedantic

clean:
	rm -f *.o main

.PHONY: clean all
