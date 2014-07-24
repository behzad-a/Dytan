/* Sample implementation of wc utility. */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void DYTAN_tag(void *addr, size_t size, char *name) {}
void DYTAN_display(void * addr, size_t size, char *fmt, ...) {}

int main (int argc, char **argv)
{
  char c, d;
  int i;
  int count;
  int total_count;
  FILE *fp;

  total_count = 0;
  
  for (i = 1; i < argc; i++) {

    count = 0;
    
    fp = fopen (argv[i], "r");

    c = fgetc(fp);
    //DYTAN_tag(&c, sizeof(c), argv[i]);
    d = c;

    while(c != EOF) {
      if(c == '\n') count++;

      c = fgetc(fp);
      //DYTAN_tag(&c, sizeof(c), argv[i]);
    }    
    fclose(fp);
    
    DYTAN_display(&count, sizeof(count), "count=%d", count);
    printf("%s: %d\n", argv[i], count);

    total_count += count;
  }

  DYTAN_display(&c, sizeof(c), "c=%c", c);
  DYTAN_display(&d, sizeof(d), "d=%c", d);
  DYTAN_display(&count, sizeof(count), "count=%d", count);
  DYTAN_display(&total_count, sizeof(total_count), "total_count=%d", total_count);
  printf("Total: %d\n", total_count);
  
  return 0;
}
