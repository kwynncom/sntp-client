sntp : main.c time.c socket.c
	gcc -O3 main.c socket.c time.c -o sntp

debug : main.c time.c socket.c
	gcc -g main.c socket.c time.c -o debug

clean : 
	rm sntp
