// -------------디렉토리를 다루기 위한 헤더----------------
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
// ------------------------------------------------------

#include <sys/stat.h>	// stat()을 쓰기 위한 헤더
#include <ctype.h>	// isdigit()를 쓰기 위한 헤더
#include <pwd.h>	// getwpuid()를 쓰기 위한 헤더
#include <string.h>	// 문자열 제어를 위한 헤더
#include <errno.h>	// 에러를 관리하기 위한 헤더
#include <stdio.h>	// fprintf를 위한 표준입출력 헤더

#include "details.h"	// 구조체가 정의된 헤더

int IsDigit(char *str) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if (isdigit(str[i]) == 0) return 0;
	}
	return 1;
}

// 읽은 프로세스의 개수를 반환
// 오류 시 -1 반환
// processInfo에 자신이 읽은 프로세스 정보를 전달
int readProcess(struct pinfo* processInfo) {
	DIR* dirp;	// opendir에 쓸 DIR 구초체 포인터
	struct dirent* direntp;	// readdir에 쓸 dirent 구조체 포인터

	char statPath[PATH_MAX];		// 프로세스의 stat 정보에 접근할 수 있는 경로
	char cmdPath[PATH_MAX];		// 프로세스의 cmdline 정보에 접근할 수 있는 경로
	struct stat lstat;	// stat에 쓸 stat 구조

	int fd;		//open에 쓸 파일 디스크립터
	struct passwd* userInfo;

	int i = 0;	// 반복자
	// "/proc"디렉토리를 연다.
	if ((dirp = opendir("/proc")) == NULL) {
		perror ("Failed to open proc directory");
	}
 
	// "/proc" 안의 디렉토리 엔트리를 끝까지 확인한다.
	while ((direntp = readdir(dirp)) != NULL) {
		// 디렉토리 엔트리가 ".", ".."이 아니라면
		if (strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..")) {
			// statPath를 "/proc/(d_name)/stat", cmdPath를 "/proc/(d_name)/cmdline"으로 한다.
			sprintf(statPath, "/proc/%s/stat", direntp->d_name);
			sprintf(cmdPath, "/proc/%s/exe", direntp->d_name);
			if (IsDigit(direntp->d_name) && access(cmdPath, R_OK) == 0) {
				// 파일위치를 구하기 위해 cmdPath를 연다
				// 프로세스가 실행된 위치를 processInfo[i][0]에 저장한다.
				if (readlink(cmdPath, processInfo[i].path, PATH_MAX) == -1) {
					perror("Failed to read cmdPath contents");
				}
				
				// lstat에 파일 정보를 가져온다.
				stat(statPath, &lstat);
				// 유저 아이디를 구해 processInfo[i][1]에 저장한다.
				strcpy(processInfo[i].username, getpwuid(lstat.st_uid)->pw_name);
				strcpy(processInfo[i].pid, direntp->d_name);
				i++;
				
			}
		}
	}
	// "/proc"을 닫는다.
	while ((closedir(dirp) == -1) && (errno == EINTR));
	// 읽은 프로세스 개수를 반환한다.	
	return i;
}
