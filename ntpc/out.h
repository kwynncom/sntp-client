#define hubufsz 200
#define obuffullsz 1500

void hutime(const unsigned long Uus, char *obuf);
void myout20f(	const unsigned long a1, 
				const unsigned long a2, 
				const unsigned long a3, 
				const unsigned long a4,
				const char *ip,
				int stratum,
				const char *refid,
				const bool isd,
				char *outb
);
