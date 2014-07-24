CC=gcc
CFLAGS=-O0 -g

copy: copy.o
	$(CC) $(CFLAGS) -o copy copy.o
	objdump -d copy > copy.asm

copy.o: copy.c
	$(CC) $(CFLAGS) -c copy.c

clean:
	rm -rf *.o copy
