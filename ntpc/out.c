#include <time.h>   // timespec struct and human time
#include <stdbool.h>
#include <math.h> // floor
#include <stdio.h> // sprintf
#include <stdlib.h> // FILE ???

#include "all.h"


void myout20f(	
	
	char *outb,
	const unsigned long a1, 
	const unsigned long a2, 
	const unsigned long a3, 
	const unsigned long a4,
	const char *ip,
	const int stratum,
	const char *refid,
	const bool isd,
	const bool psck

) {

	char *sd;
	if (psck) sd =  "**OK** - C - passes sanity check";
	else	  sd = "fails C sanity check";


    const char *fmt = "%lu\n%lu\n%lu\n%lu\n%s\nStratum: %d\nRefID: %s\nVERSION: %s\n%s\n";
    sprintf (outb, fmt, a1, a2, a3, a4, ip, stratum, refid, KWSNTPV, sd);
}

void hutime(const unsigned long Uus, char *obuf) {
	time_t rawtime;
	if (!Uus) rawtime = time(NULL);
	else      rawtime = (unsigned long)floorl(Uus / M_BILLION);
	struct tm *t = localtime(&rawtime);

	const char *fmt =  "%02d:%02d:%02d %02d/%02d/%04d %ld";
	sprintf(obuf, fmt, t->tm_hour, t->tm_min, t->tm_sec, t->tm_mon + 1, t->tm_mday, t->tm_year + 1900, Uus ? Uus : rawtime );
}
