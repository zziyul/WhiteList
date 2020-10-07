#include "details.h"
#include <unistd.h>
#include <time.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readProcess(struct pinfo* processInfo);
int readWhiteList(struct winfo* whiteListInfo);
int writeWhiteList(struct winfo* info, int n, int option);
 
void WhiteListEditor(int option);
 
int main(int argc, char* argv[]) {
	if (argc == 2) {
		WhiteListEditor(atoi(argv[1]));
	}
	else {
		fprintf(stderr, "잘못된 인자를 입력했습니다.");
		return -1;
	}
	return 0;
}
void WhiteListEditor(int option) {
	struct pinfo processInfo[PROCESS_MAX]; // 현재실행중인 프로세스
	struct winfo whitelistInfo[PROCESS_MAX]; // 화이트리스트 프로세스
	struct winfo newWhiteList[PROCESS_MAX];		// 추가할 화이트리스트 프로세스 목록
 
	int numOfProcess; // 읽어온 프로세스
	int numOfWhitelist; // 읽어온 화이트리스트
	int numOfNew;
 
 
	int i, j; // 반복자
 
	// 관련 정보를 읽어온다.
	numOfProcess = readProcess(processInfo);
	numOfWhitelist = readWhiteList(whitelistInfo);
	numOfNew = 0;

	//옵션에 따라 실행
	switch(option){
		// 옵션으로 초기화하고 현재 프로세스만 입력
		case 0:
			//현재 프로세스를 화이트리스트에 입력
			for(i = 0; i < numOfProcess; i++) {
				strcpy(newWhiteList[i].path, processInfo[i].path);
				strcpy(newWhiteList[i].username, processInfo[i].username);
			}
			writeWhiteList(newWhiteList, numOfProcess, 0);
			break;
		//현재프로세스 반영해 보강 
		case 1:
			// processInfo와 whitelistInfo를 비교한다.
			// processInfo에서 프로세스 실행위치와 whitelistInfo에서 프로세스 실행위치를 비교한다.
			for (i = 0; i < numOfProcess; i++) {
				for (j = 0; j < numOfWhitelist; j++) {
					// 만약 같은게 나온다면 break
					if (!strcmp(processInfo[i].path, whitelistInfo[j].path) && !strcmp(processInfo[i].username, whitelistInfo[j].username)) {
						break;
					}
				}
				if (j != numOfWhitelist) continue;
				strcpy(newWhiteList[numOfNew].path, processInfo[i].path);
				strcpy(newWhiteList[numOfNew].username, processInfo[i].username);
				numOfNew++;
			}
			writeWhiteList(newWhiteList, numOfNew, option);
			break; 

	}

}
