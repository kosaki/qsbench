
CC = gcc
CFLAGS = -O2 -Wall -Wstrict-prototypes -g

all: qsbench bsbench

qsbench: qsbench.c
	$(CC) -o qsbench qsbench.c $(CFLAGS)

bsbench: bsbench.c
	$(CC) -o bsbench bsbench.c $(CFLAGS)

clean:
	rm -f *.o qsbench
