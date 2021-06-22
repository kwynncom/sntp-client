#include <stdio.h>
#include <stdlib.h> // exit()
#include "sntp.h"
#include "config.h"

void main(void) {
	sntp_doit(KW_DEFAULT_NTP_N_POLLS, KW_DEFAULT_NTP_SERVER);
}
