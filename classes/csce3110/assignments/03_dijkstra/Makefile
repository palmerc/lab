all: main

%: %.cpp
	gcc -lboost_regex -o $@ $< -g -Wall

clean:
	rm -f *.o main

.PHONY: clean all
