#include <stdio.h>
#include <unistd.h> // read
#include <string.h> // strstr
#include <stdlib.h> // struct timespec
#include <fcntl.h> // open
#include "all.h"
#define crwversion "2023/01/20 02:57 - one char"


bool shouldCloseF(const int argc, const char *argv[]) {
	for (int i=1; i < argc; i++) {
		if (strcmp(argv[i],  "x") == 0) return true;
		if (strcmp(argv[i], "-x") == 0) return true;		
	}

	return false;
}

void mywrite(const bool sc) {
	char *b = "a";
	if (sc) b = "x";
	const int sz = strlen(b);
	const int wd = open(KWSNTPDPOKE, O_WRONLY | O_NONBLOCK);
	write(wd, b, sz);
	close(wd);	
}

void myread() {
	int rd;
	const int sz = KWSNTPMINOUTLEN;
	char    b  [sz + 1];
	char    igb[sz + 1]; // ignore buffer - no need to zero it
	bzero(b,  sz + 1);
	
	const int stepsn = 14;  
	const int steps   [14]  = {  2, 3, 2, 2, 3, 40, 20, 40, 50, 50, 100, 100, 100, 100};

	rd = open(KWSNTPDEXTGET, O_RDONLY | O_NONBLOCK);
	int i=0;

	for (i=0; i < stepsn; i++) {
		usleep(steps[i] * 1000);
		if (read(rd, b, sz) >= sz) break;
	}

	read(rd, igb, sz); // might need to clear any unused output

	close(rd);
	printf("%s", b);
	printf("%s %s\n", "C FIFO runner / wrapper VERSION: ", crwversion);
	// printf("******************\n");
}

void main(const int argc, const char *argv[]) { // I will need to account for the exit "x" rather than non-x
	mywrite(shouldCloseF(argc, argv));
	myread();
}
