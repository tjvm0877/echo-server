CC      := gcc
CFLAGS  := -O2 -Wall -I .
LDFLAGS := 
LDLIBS  := -pthread

COMMON_OBJS := echo.o csapp.o

SRV_MP_OBJ  := echoserver-mp.o
SRV_MT_OBJ  := echoserver-mt.o
SRV_MUX_OBJ := echoserver-mux.o
CLI_OBJ     := echoclient.o

TARGETS := echoserver-mp echoserver-mt echoserver-mux echoclient

.PHONY: all clean

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

echoserver-mp: $(SRV_MP_OBJ)  $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

echoserver-mt: $(SRV_MT_OBJ)  $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

echoserver-mux: $(SRV_MUX_OBJ) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

echoclient: $(CLI_OBJ) csapp.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *~ *.o $(TARGETS) core *.tar *.zip
