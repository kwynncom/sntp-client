sntp : main.c utils.c call.c
	gcc -O3 main.c utils.c call.c -o sntp

debug : main.c utils.c
	gcc -g main.c utils.c -o debug

clean : 
	rm sntp
