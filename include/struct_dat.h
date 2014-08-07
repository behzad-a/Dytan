#ifndef struct_dat__
#define struct_dat__
typedef struct _datFile {
	int index;
	int start[256];
	int end[256];
} datfile;

extern datfile datFile;
#endif

#include <deque>

#ifndef struct_rules__
#define struct_rules__
typedef struct _rules {
	int returnIndex;
	std::deque<int> srcArgs;
	std::deque<int> destArgs;
} rules;
#endif
