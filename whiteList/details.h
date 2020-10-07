#include <time.h>
// -------------버퍼의 크기를 지정해 줄 기호상수------------
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 200
#endif

#ifndef PROCESS_MAX
#define PROCESS_MAX 200
#endif

#ifndef NAME_MAX
#define NAME_MAX 16
#endif

#ifndef PID_MAX
#define PID_MAX 10
#endif

//9~13
#define SEPERATOR 9
// ------------------------------------------------------

struct pinfo {
	char path[PATH_MAX];
	char username[NAME_MAX];
	char pid[PID_MAX];
};

struct winfo {
	char path[PATH_MAX];
	char username[NAME_MAX];
};

struct rinfo {
	char path[PATH_MAX];
	char username[NAME_MAX];
	char detectedTime[11];	// time_t는 long 타입을 저장하고 있기 때문에 최대 2147483647까지 저장하므로 널문자까지 11자리가 필요하다.
};
