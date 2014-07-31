#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

struct _config {
        int test;
	char dir[256];
	char filename[256];
	int mode;
} config;

void DYTAN_tag(char *start, size_t size, char *name)
{
}

void DYTAN_display(char* start, size_t size, char *fmt, ...)
{
}

int parse_conf(char *buf)
{
	char *token = strtok(buf, "=\n");
	int count = 0;
	int ret = 0;
	int option;
	while (token) {
		if (count % 2 == 0) {
			// name
			if (strcmp(token, "dir") == 0)
				option = 0;
			else if (strcmp(token, "filename") == 0)
				option = 1;
			else if (strcmp(token, "mode") == 0)
				option = 2;
			else if (strcmp(token, "test") == 0)
				option = 3;
			else {
				//printf("Invalid option:%s\n", token); 
				//ret = -1;
				option = -1;
			}
		} else {
			// value
			switch(option) {
			case 0:
				strcpy(config.dir, token);
				break;
			case 1:
				strcpy(config.filename, token);
				break;
			case 2:
				config.mode = atoi(token);
				break;
			case 3:
				config.test = atoi(token);
				break;
			default:
				break;
			}
		}
		token = strtok(NULL, "=\n");
		count ++;
	}
	return ret;
}

int main(int argc, char *argv[]) {
  int i = 5;
  int j;
  int k;
  char buf[256];
  int count;

  printf("i:%p, j:%p, k:%p, buf:%p, config:%p\n", &i, &j, &k, buf, &config);
  //DYTAN_tag((char *)&i, sizeof(i), "i");
  j = i * 2;
  k = i / 2;
  printf("%d %d %d\n", i, j, k);

  FILE *fp = NULL;

  fp = fopen("conf", "rt");
  if (fp) {
	  memset(buf, 0, sizeof(buf));
	  count = fread(buf, 1, sizeof(buf), fp);
	  if (count == 0)
		printf("Error: empty conf file!\n");
	  //else
		//printf("%s\n", buf);
		  
	  fclose(fp);
	  parse_conf(buf);
	  printf("dir=%s\nfilename=%s\nmode=%d\ntest=%d\n", config.dir, config.filename, config.mode, config.test);
	  printf("config.dir at %p\nconfig.filename at %p\nconfig.mode at %p\nconfig.test at %p\n", &(config.dir), &(config.filename), &(config.mode), &(config.test));
  } else
	  printf("Error: cannot open conf file!\n");
  //DYTAN_display(buf, count, "Buf");

  return 0;
}
