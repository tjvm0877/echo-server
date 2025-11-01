CC = gcc
CFLAGS = -O2 -Wall -I .
LIB = -pthread

.PHONY: all clean

all: echoserver echoclient

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

echo.o: echo.c csapp.h
	$(CC) $(CFLAGS) -c echo.c

echoserver.o: echoserver.c csapp.h
	$(CC) $(CFLAGS) -c echoserver.c

echoclient.o: echoclient.c csapp.h
	$(CC) $(CFLAGS) -c echoclient.c

echoserver: echoserver.o echo.o csapp.o
	$(CC) $(CFLAGS) echoserver.o echo.o csapp.o -o echoserver $(LIB)

echoclient: echoclient.o csapp.o
	$(CC) $(CFLAGS) echoclient.o csapp.o -o echoclient $(LIB)

clean:
	rm -f *~ *.o echoserver echoclient core *.tar *.zip
