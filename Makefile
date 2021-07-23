sntp : main.c utils.c
	gcc -O3 main.c utils.c -o sntp

debug : main.c utils.c
	gcc -g main.c utils.c -o debug

clean : 
	rm sntp
