sntp:					  *.c *.h 
		gcc  -O3  -static main.c utils.c call.c -o sntp  -lm
		gcc   -g  -static main.c utils.c call.c -o debug -lm
		gcc -g w.c -o w

clean:
	rm -f sntp debug
