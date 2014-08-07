CC=gcc
CFLAGS=-O0 -g

wc: wc.o
	$(CC) $(CFLAGS) -o wc wc.o
	objdump -d wc > wc.asm

wc.o: wc.c
	$(CC) $(CFLAGS) -c wc.c

clean:
	rm -rf *.o wc
