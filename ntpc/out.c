#include <time.h>   // timespec struct and human time
#include <stdbool.h>
#include <math.h> // floor
#include <stdio.h> // sprintf
#include <stdlib.h> // FILE ???

#include "all.h"


void myout20f(	const unsigned long a1, 
				const unsigned long a2, 
				const unsigned long a3, 
				const unsigned long a4,
				const char *ip,
				int stratum,
				const char *refid,
				const bool isd,
				char *outb
) {
    const char *fmt = "%lu\n%lu\n%lu\n%lu\n%s\nStratum: %d\nRefID: %s\n";
    sprintf (outb, fmt, a1, a2, a3, a4, ip, stratum, refid);
}

void hutime(const unsigned long Uus, char *obuf) {
	time_t rawtime;
	if (!Uus) rawtime = time(NULL);
	else      rawtime = (unsigned long)floorl(Uus / M_BILLION);
	struct tm *t = localtime(&rawtime);

	const char *fmt =  "%02d:%02d:%02d %02d/%02d/%04d %ld"; // \nVERSION: %s\n";
	// printf("VERSION: %s\n", KWSNTPV);

	
	sprintf(obuf, fmt, t->tm_hour, t->tm_min, t->tm_sec, t->tm_mon + 1, t->tm_mday, t->tm_year + 1900, rawtime /*, KWSNTPV */);
}
