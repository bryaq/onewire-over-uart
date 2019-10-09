OBJS   += onewire.o
CC     ?= gcc
CFLAGS += -O2 -Wall

ifeq ($(OS), Windows_NT)
	OBJS += uart_win.o
else
	OBJS += uart_posix.o
endif

OBJS1 = $(OBJS) lsow.o
OBJS2 = $(OBJS) digitemp_evil.o devices/common.o devices/ds18x20.o
OBJS3 = $(OBJS) rw1990.o devices/common.o
OBJS4 = $(OBJS) ds1990.o devices/common.o
OBJS5 = $(OBJS) rw2004.o devices/common.o

all: lsow digitemp_evil rw1990 ds1990 rw2004

lsow: $(OBJS1)
	$(CC) -static -O3 -o $@ $(OBJS1)

digitemp_evil: $(OBJS2)
	$(CC) -static -O3 -o $@ $(OBJS2)

rw1990: $(OBJS3)
	$(CC) -static -O3 -o $@ $(OBJS3)

ds1990: $(OBJS4)
	$(CC) -static -O3 -o $@ $(OBJS4)

rw2004: $(OBJS5)
	$(CC) -static -O3 -o $@ $(OBJS5)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS1) $(OBJS2) uart_*.o lsow digitemp_evil README.html

doc: README.md
	markdown README.md > README.html

install: lsow digitemp_evil
	install -m 0755 lsow /usr/local/bin/lsow
	install -m 0755 digitemp_evil /usr/local/bin/digitemp_evil

uninstall:
	rm /usr/local/bin/lsow /usr/local/bin/digitemp_evil
