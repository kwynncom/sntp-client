sntp : main.c sntp.c time.c socket.c
	gcc -g main.c sntp.c time.c socket.c -lm -o sntp

clean : 
	rm sntp
