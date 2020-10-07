// ----------------------파일을 다루기 위한 헤더-------------------------
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
// --------------------------------------------------------------------

#include <string.h>	// 문자열 제어를 위한 헤더
#include <stdio.h>	// fprintf를 위한 표준입출력 헤더
#include <errno.h>	// 에러를 관리하기 위한 헤더
#include "details.h"	// 구조체가 정의된 헤더

int writeWhiteList(struct winfo* info, int n, int option) {
	char seperator[2];
	int fd;	// 파일 디스크립터
	struct winfo* whitelistInfo;	// 현재 화이트리스트에 있는 정보
	int k;	// 화이트리스트에서 얻어온 정보의 개수
	int i;	// 반복자
	int j;	// 반복자
	int added;	// 덧붙일지 말지 결정하는 변수
	
	seperator[0] = SEPERATOR;
	seperator[1] = '\0';

	// 화이트리스트를 초기화하고 새로운 화이트리스트를 만드는 옵션
	if (option == 0) {
		// whitelist가 없다면 만들어서 쓰기 전용으로 연다.
		// whitelist의 내용은 초기화시키고 처음부터 쓰기 시작한다.
		if((fd = open("whitelist", O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO)) == -1) {
			perror("Failed to open whitelist");
			return -1;
		}
	}
	// 기존의 화이트리스트에 덧붙이는 옵션
	else if (option == 1) {
		// whitelist가 없다면 만들어서 쓰기 전용으로 연다.
		// whitelist의 끝부분부터 덧붙여 쓰기 시작한다.
		if((fd = open("whitelist", O_WRONLY | O_APPEND | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO)) == -1) {
			perror("Failed to open whitelist");
			return -1;
		}
	}
	else {
		perror("허용되지 않은 옵션을 사용했습니다");
		return -1;
	}
	
	for (i = 0; i < n; i++) {
		write(fd, info[i].path, strlen(info[i].path));
		write(fd, seperator, 1);
		write(fd, info[i].username, strlen(info[i].username));
		write(fd, "\n", 1);
	}
	return 0;
}
