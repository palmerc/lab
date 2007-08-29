#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
	char line[256];

	fgets(line, 256, stdin);
	execl("/bin/sh", "sh", "-c", line, (char *)0); 

	return 0;
}
