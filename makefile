CC = gcc
CFLAGS = -O2 -Wall -I .
LIB = -pthread

.PHONY: all clean

all: echoserveri echoclient

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

echo.o: echo.c csapp.h
	$(CC) $(CFLAGS) -c echo.c

echoserveri.o: echoserveri.c csapp.h
	$(CC) $(CFLAGS) -c echoserveri.c

echoclient.o: echoclient.c csapp.h
	$(CC) $(CFLAGS) -c echoclient.c

echoserveri: echoserveri.o echo.o csapp.o
	$(CC) $(CFLAGS) echoserveri.o echo.o csapp.o -o echoserveri $(LIB)

echoclient: echoclient.o csapp.o
	$(CC) $(CFLAGS) echoclient.o csapp.o -o echoclient $(LIB)

clean:
	rm -f *~ *.o echoserveri echoclient core *.tar *.zip
