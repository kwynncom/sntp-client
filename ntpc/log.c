#include <stdio.h> // FILE
#include <stdbool.h>
#include <time.h>
#include "utils.h"

void calllog(bool doClose) {
	static FILE *f = NULL;

	if (doClose && f != NULL) { fclose(f); return; }
	if (f == NULL) f = fopen(LOGFILE, "a");
	time_t rawtime = time(NULL);
	struct tm *t = localtime(&rawtime);
	
	fprintf(f, "%02d:%02d:%02d %02d/%02d/%04d %ld\n", t->tm_hour, t->tm_min, t->tm_sec, t->tm_mon + 1, t->tm_mday, t->tm_year + 1900, rawtime);	
		
}