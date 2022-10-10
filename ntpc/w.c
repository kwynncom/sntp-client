#include <stdio.h>
#include <unistd.h> // read
#include <string.h> // strstr
#include <stdlib.h> // struct timespec
#include <fcntl.h> // open
#include "all.h"

void mywrite() {
	int wd;
	const char *b = "a";
	int sz = strlen(b);
	wd = open(KWSNTPDPOKE, O_WRONLY | O_NONBLOCK);
	write(wd, b, sz);
	close(wd);	
}

void myread() {
	int rd;
	const int sz = 214; // based on size of output right now
	char    b[sz + 1];
	bzero(b,  sz + 1);
	
	const int stepsn = 14;  
	const int steps   [14]  = {  2, 3, 2, 2, 3, 40, 20, 40, 50, 50, 100, 100, 100, 100};

	rd = open(KWSNTPDEXTGET, O_RDONLY | O_NONBLOCK);
	int i=0;

	for (i=0; i < stepsn; i++) {
		usleep(steps[i] * 1000);
		read(rd, b, sz);
		if (strstr(b, "**OK**") != NULL) break;
		if (strstr(b, "fails" ) != NULL) break;
	}
	close(rd);
	printf("%s\n", b);
}

void main(void) { // I will need to account for the exit "x" rather than non-x
	mywrite();
	myread();
}

// https://stackoverflow.com/questions/7360473/linux-non-blocking-fifo-on-demand-logging