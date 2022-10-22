CC=gcc -std=c11 -Wall -Werror

trove:	tools.c tools.h \
		trove.c trove.h

		
	$(CC) -c tools.c
	$(CC) -c trove.c
	$(CC) -o trove trove.c tools.o
	rm -f *.o