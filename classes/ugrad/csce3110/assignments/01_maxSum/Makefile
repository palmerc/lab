all: main

%: %.cpp
	g++ -o $@ $< -g -Wall

clean:
	rm -f *.o main 

.PHONY: clean all
