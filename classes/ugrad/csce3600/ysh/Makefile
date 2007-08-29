all: ysh

%: %.c
	gcc -o $@ $< -g -Wall

clean:
	rm -f *.o ysh

.PHONY: clean all
