
CC = gcc
CFLAGS = -O2 -Wall -Wstrict-prototypes

all: qsbench bsbench qsbench-kosa

qsbench: qsbench.c
	$(CC) -o qsbench qsbench.c $(CFLAGS)

bsbench: bsbench.c
	$(CC) -o bsbench bsbench.c $(CFLAGS)

qsbench-kosa: qsbench-kosa.c tmpfile.c
	$(CC) -o qsbench-kosa qsbench-kosa.c tmpfile.c $(CFLAGS)

clean:
	rm -f *.o qsbench
