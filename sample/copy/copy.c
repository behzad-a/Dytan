/* Sample implementation of copy utility. */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void DYTAN_tag(void *addr, size_t size, char *name) {}
void DYTAN_display(void * addr, size_t size, char *fmt, ...) {}

int main (int argc, char **argv)
{
  char inbuf[16], outbuf[16];
  int fd_src, fd_dest;
  char *src, *dest;
  int len, err = 0;

  if (argc < 3) {
	printf("Usage: copy src dest\n");
	return 0;
  }
  src = argv[1];
  dest = argv[2];
  fd_src = open(src, O_RDONLY);
  if (fd_src == -1) {
	printf("Unable to open %s\n", src);
	return 0;
  }
  fd_dest = open(dest, O_CREAT|O_TRUNC|O_WRONLY, 0666);
  if (fd_dest == -1) {
	printf("Unable to create %s\n", dest);
	return 0;
  }
  while (1) {
  	len = read(fd_src, inbuf, sizeof(inbuf));
	if (len == 0)
		break;
	else if (len < 0) {
		printf("Failed to read %s\n", src);
		err = 1;
		break;
	}
	//DYTAN_tag(inbuf, len, argv[0]);
	memcpy(outbuf, inbuf, len);
	if (write(fd_dest, outbuf, len) != len) {
		printf("Error writing to %s\n", dest);
		err = 1;
		break;
	}
  }
  close(fd_src);
  close(fd_dest);
  if (err)
	unlink(dest);

  DYTAN_display(inbuf, sizeof(inbuf), "inbuf=%s", inbuf);
  DYTAN_display(outbuf, sizeof(outbuf), "outbuf=%s", outbuf);
  
  return 0;
}
