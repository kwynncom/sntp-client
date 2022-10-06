#include <stdio.h> // FILE
#include <stdbool.h>
#include <time.h>
#include <math.h> // floorl
#include "utils.h"

void calllog(bool doClose, bool toFile, bool newCall, unsigned long Uus) {
	static FILE *f = NULL;
	int fpfr = 0;
	char *fmt =  "%02d:%02d:%02d %02d/%02d/%04d %ld %s %s\n";

	char *tofs = toFile  ? "" : "(stdout only)";
	char *ncs  = newCall ? "" : "(cached)";

	if (doClose && f != NULL) { fclose(f); return; }
	if (f == NULL) f = fopen(LOGFILE, "a");

	time_t rawtime;
	if (!Uus) rawtime = time(NULL);
	else      rawtime = (unsigned long)floorl(Uus / M_BILLION);
	struct tm *t = localtime(&rawtime);
	
	if (toFile) {
		fprintf(f, fmt, t->tm_hour, t->tm_min, t->tm_sec, t->tm_mon + 1, t->tm_mday, t->tm_year + 1900,	   rawtime, ncs, tofs);	
		fflush(f);
	} else printf(    fmt, t->tm_hour, t->tm_min, t->tm_sec, t->tm_mon + 1, t->tm_mday, t->tm_year + 1900, rawtime, ncs, tofs);
}
