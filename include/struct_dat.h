#ifndef struct_dat__
#define struct_dat__
typedef struct _datFile {
	int index;
	int start[256];
	int end[256];
} datfile;

extern datfile datFile;
#endif
