all: test

%: %.cpp
	gcc -lboost_regex -o $@ $< -g -Wall

clean:
	rm -f *.o test

.PHONY: clean all
