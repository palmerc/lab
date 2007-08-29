all: dijkstra

%: %.cpp
	gcc -lboost_regex -o $@ $< -g -Wall

clean:
	rm -f *.o dijkstra

.PHONY: clean all
