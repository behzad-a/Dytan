#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>

struct _config {
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
			default:
				break;
			}
		}
		token = strtok(NULL, "=\n");
		count ++;
	}
	return ret;
}

int load_config()
{
  int i = 5;
  int j;
  int k;
  char buf[256];
  int count;

  printf("i:%p, j:%p, k:%p, buf:%p, config:%p\n", &i, &j, &k, buf, &config);
  DYTAN_tag((char *)&i, sizeof(i), "i");
  j = i * 2;
  k = i / 2;
  printf("%d %d %d\n", i, j, k);

  FILE *fp = NULL;
  int ret = 0;

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
	  printf("dir=%s\nfilename=%s\nmode=%d\n", config.dir, config.filename, config.mode);
  } else {
	  ret = -1;
	  printf("Error: cannot open conf file!\n");
  }
  DYTAN_display(buf, count, "Buf");
  return ret;
}

void do_work() {
  if (config.mode == 0)
	printf("%s\n%s\n", config.dir, config.filename);
  else if (config.mode == 1)
	printf("%s\n%s\n", config.filename, config.dir);
}

#define NUM_THREADS 2
#define TASK_AMOUNT 10000

void *thread_func(void *arg) {
	int sum = 0;
	int i;

	for (i = 0; i < TASK_AMOUNT; i++)
		sum += i;
	printf("%x: sum is %d\n", (int)pthread_self(), sum);
}

int main(int argc, char *argv[]) {
#if 0
  char buf1[16], buf2[16];
  int i;

  for (i = 0; i < sizeof(buf1); i++) {
 	char name[16];
	sprintf(name, "%d", i);
   	DYTAN_tag(&buf1[i], 1, name);
  }
  memcpy(buf2, buf1, sizeof(buf1));
  DYTAN_display(buf1, sizeof(buf1), "buf1");
  DYTAN_display(buf2, sizeof(buf2), "buf2");
#endif
  pthread_t thread[NUM_THREADS];
  int i, num_threads;

  if (argc > 1) {
	num_threads = atoi(argv[1]);
	if (num_threads < 0 || num_threads > NUM_THREADS) {
		printf("Invalid thread number %d\n", num_threads);
		return 1;
	}
	printf("Start with %d helper threads...\n", num_threads);
  }

  for (i = 0; i < num_threads; i++) {
  	if (pthread_create(&thread[i], NULL, thread_func, NULL)) {
		printf("Error creating thread %d\n", i);
		break;
	}
  }
  if (load_config() == 0)
  	do_work();
  for (i = 0; i < NUM_THREADS; i++) {
	pthread_join(thread[i], NULL);
  }
  return 0;
}
