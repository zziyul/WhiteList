#include "details.h"
#include <unistd.h>
#include <time.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readProcess(struct pinfo* processInfo);
int readWhiteList(struct winfo* whiteListInfo);
int readReport(struct rinfo* reportInfo);
int writeReport(struct rinfo* reportInfo, int n, int option);

void ProcessInspector(int option);

int main(int argc, char* argv[]) {
	if (argc == 2) {
		ProcessInspector(atoi(argv[1]));
		
	}
	else {
		fprintf(stderr,"잘못된 인자를 입력했습니다.");
	}
	return 0;
}
void ProcessInspector(int option) {
	struct pinfo processInfo[PROCESS_MAX];	// 현재 실행되고 있는 프로세스 정보
	struct winfo whitelistInfo[PROCESS_MAX];	// 화이트리스트에 있는프로세스 정보
	struct rinfo reportInfo[PROCESS_MAX];	// 리포트에 있는 프로세스 정보
	
	struct rinfo newreport[PROCESS_MAX];

	int numOfProcess;	// 읽어온 프로레스 개수
	int numOfWhitelist;		// 읽어온 화이트리스트 개수
	int numOfReport;	// 읽어온 리포트 항목 개수
	int numOfNew;		// 리포트에 저장할 항목 개

	time_t ctime;	// 최초 발견 시간
	char detectedTime[11];	// reportInfo에 저장할 시간.

	int i, j, k;	// 반복자

	// 관련 정보를 읽어온다.
	numOfProcess = readProcess(processInfo);
	numOfWhitelist = readWhiteList(whitelistInfo);
	numOfReport = readReport(reportInfo);
	numOfNew = 0;
	
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
		// 옵션이 1이라면 report에도 없는 녀석만을 따로 선별
		if (option == 1) {
			for (k = 0; k < numOfReport; k++) {
				if (!strcmp(processInfo[i].path, reportInfo[k].path) && !strcmp(processInfo[i].username, reportInfo[k].username)) {
					break;
				}
			}
			if (k != numOfReport) continue;
		}
		strcpy(newreport[numOfNew].path, processInfo[i].path);
		strcpy(newreport[numOfNew].username, processInfo[i].username);
		ctime = time(NULL);
		sprintf(newreport[numOfNew].detectedTime, "%ld", ctime);
		numOfNew++;
	}
	writeReport(newreport, numOfNew, option);

	//processInfo와 whitelistInfo의 비교가 끝나면 ProcessJudge를 실행시킨다.
	//pid_t pid;
	//pid = fork();
	//	if (pid == 0) {
	//	execl('ProcessJudge의 path', (char*)0);//ProcessJudge 실행
	//}
}
