all: kmb

%: %.cpp
	gcc -lboost_regex -o $@ $< -g -Wall

clean:
	rm -f *.o kmb

.PHONY: clean all
