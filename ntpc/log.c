#include <stdio.h> // FILE
#include <stdbool.h>
#include <time.h>
#include <math.h> // floorl
#include "all.h"

void calllog(const bool newCall, const unsigned long Uus, const bool doClose ) {
	static FILE *f = NULL;
	static unsigned long prevts = 1;

	int fpfr = 0;
	char *fmt =  "%02d:%02d:%02d %02d/%02d/%04d %ld %s\n";

	if (doClose && f != NULL) { fclose(f); return; }
	if (f == NULL) f = fopen(LOGFILE, "a");

	time_t rawtime;
	if (!Uus) rawtime = time(NULL);
	else      rawtime = (unsigned long)floorl(Uus / M_BILLION);
	struct tm *t = localtime(&rawtime);
	
	if (newCall) {
		fprintf(f, fmt, t->tm_hour, t->tm_min, t->tm_sec, t->tm_mon + 1, t->tm_mday, t->tm_year + 1900,	   rawtime, "");	
		fflush(f);
	} else {
		char *cs = "";
		if (Uus == prevts) cs = "(cached)";
		printf(    fmt, t->tm_hour, t->tm_min, t->tm_sec, t->tm_mon + 1, t->tm_mday, t->tm_year + 1900, Uus, cs);
		prevts = Uus;
	}
}
