# sntp-client

SNTP (network time) client

REQUIREMENTS - gcc, php
 
BASIC USAGE

make
php wrap.php

EXPECTED OUTPUT

1,627,005,339,012,806,912
1,627,005,339,031,896,320
1,627,005,339,032,008,704
1,627,005,339,049,829,376

4 UNIX Epoch nanoseconds: 

1. client time before request, 
2. server time upon request receipt
3. server time upon send
4. client time upon receipt

Note that I'm writing this with the number_format().  The ',' will probably go away.

Running the binary directly results in gibberish / binary characters.

TECH NOTES

Note that I am not populating the SNTP packet with client time--neither send nor receive.  I am tracking it separately.  The servers work just 
fine without it, it's more trouble than it's worth to populate, and as long as I'm tracking it somewhere I don't need it in the packet.

The ::FFFF: prefix allows the IP4 address to work in IP6.
48 is an SNTP packet length
The "#" sent to the server is interpreted as binary.  It's 0x23.  It defines SNTP v4 (VN) and defines the packet as coming from a client (Mode).
See https://datatracker.ietf.org/doc/html/rfc4330#page-8

The packet header is
LI   - 2 bit leap second indicator; always 0 from client
VN   - 3 bit SNTP version number; 4 is current
Mode - 3 bit packet type where 3 = client

So that's 8 bits.  We want

LI VN  Mode
00100011 === 0x23 === ASCII '#' https://www.asciitable.com/



NTP SERVERS

Caution: there are limits on how often you should poll.  NIST no more than 4s.  The NTP pool says 64s.

Official NIST: https://tf.nist.gov/tf-cgi/servers.cgi
Several servers, including NIST - https://github.com/kwynncom/code-fragments/blob/2ab0fe31ca991766094f5b3ae67b0b65908dd9cc/sntp/config_servers.php

HISTORY

2021/07/22 9:21pm EDT / UTC -4 / America/New_York

I am overwriting much of my previous version.  This new one is much simpler.
