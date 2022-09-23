sntp :          main.c utils.c call.c quota.c
	gcc -O3 main.c utils.c call.c quota.c -o sntp

debug :        main.c utils.c call.c quota.c
	gcc -g main.c utils.c call.c quota.c -o debug

clean : 
	rm sntp debug

