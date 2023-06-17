#define KWSNTPV "2023/06/16 20:42 - 4 CO IPv6"

#define KWSNTPDLOCKFILE "/var/kwynn/mysd/lockC" // must match PHP
#define KWSNTPDEXTGET   "/var/kwynn/mysd/get"
#define KWSNTPDPOKE		"/var/kwynn/mysd/poke"
#define LOGFILE			"/var/kwynn/mysd/logC.txt"
#define ALLRESULTSFILE  "/var/kwynn/mysd/fullLog.txt"

#define NISTMaxS 4.0

// 11 are the Maryland 7 + 4x IPv6 Colorado
// #define IPN 11 // 7 NIST server IPs by default ; 5 are IPv4; 2 are IPv6, so the next default

#define IPN 4 // 4 === IPv6 Colorado


#define M_BILLION 1000000000
#define M_MILLION 1000000

#define SNPL   48 // SNTP packet length
#define SNTPREFIDSTART 12
#define REFIDSZ 5 // including \0
#define KWSNBR "KWBR"
#define KWSNBW "KWBW"

#define MAXIPL 40 // full IPv6 addr plus \0
#define MINIPL  3 // ::1 is 3 chars

#define TOLERANCENS M_BILLION
#define KWSNTPMINOUTLEN 2

#include <stdbool.h>

void calllog(const bool newCall, const unsigned long Uus, const bool doClose, FILE *genoutf);
unsigned long nanotime();
bool setAddr(const char *ipin, char *ipout);
int getOutboundUDPSock(const char *addrStr, const int port);
void setOBPack(char *pack);
long double Ufl();

struct sockInfo {
    char iphu[MAXIPL];
	char ip46[MAXIPL];
    int  sock;
	bool alwaysQuota;
};

bool popSocks(struct sockInfo *socks, const char *ip, const bool isd);
void procArgs(int argc, char *argv[], bool *isd, bool *dosleep, bool *qck, char (*ip)[MAXIPL]);
FILE *getLockedFile();
void cleanup(const struct sockInfo *socks, const FILE *lockf);
void mysleep();

bool sanityCheck(const unsigned long a, const unsigned long b, const unsigned long c, const unsigned long d);

bool myoutf(const struct timespec bs, const struct timespec es, const char *pack, const char *ip, const bool isd);

bool qckf();
bool onin(void);
