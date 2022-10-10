#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
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
	char b[sz + 1];
	rd = open(KWSNTPDEXTGET, O_RDONLY | O_NONBLOCK);
	
	int i=0;
	const int  stepsn  =  7;
	const int steps[7]  = {2, 5, 5, 40, 50, 200, 700};


	for (i=0; i < stepsn; i++) {
		usleep(steps[i] * 1000);
		read(rd, b, sz);
		if (strstr(b, "**OK**") != NULL) break;
		if (strstr(b, "fails" ) != NULL) break;
	}
	close(rd);
	printf("%s\n", b);
	printf("%d\n", i);
}

void main(void) { // I will need to account for the exit "x" rather than non-x
	mywrite();
	myread();
}

// https://stackoverflow.com/questions/7360473/linux-non-blocking-fifo-on-demand-logging