CC=gcc
CFLAGS=-g
LIBS=#-lcurl

fetch: http_client.c
	$(CC) $(CFLAGS) http_client.c -o fetch $(LIBS)

clean:
	rm -rf *.o fetch *.out *.log *.html
