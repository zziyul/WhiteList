#include "details.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

//reportInfo를 통하여 각각의 항목에 대해 stdin입력을 받아 
//kill, unlink, 화이트 리스트에 포함, 무시 중 하나 선택하게 한다.

int readProcess(struct pinfo* processInfo);
int readWhiteList(struct winfo* whiteListInfo);
int readReport(struct rinfo* reportInfo);
int writeWhiteList(struct winfo* info, int n, int option);
int writeReport(struct rinfo* info, int n, int option);
int deleteReport(struct rinfo* info, int n);


int main(int argc, char* argv[]) {
	struct pinfo processInfo[PROCESS_MAX];
	struct winfo whiteListInfo[PROCESS_MAX];
	struct rinfo reportInfo[PROCESS_MAX];
	struct winfo newWhiteList;
	struct rinfo newReport;
	
	int numOfProcess;
	int numOfReport;
	int numOfWhiteList;
	
	int i, j, k;	// 반복자
	int num; // 입력을 위한 int형 변수	

	numOfProcess = readProcess(processInfo);
	numOfWhiteList = readWhiteList(whiteListInfo);
	numOfReport = readReport(reportInfo);
	for (i = 0; i < numOfReport; i++) {
		printf("\"%s\"를 어떻게 하시겠습니까?\n", reportInfo[i].path);
		printf("1. 프로세스 종료 2. 프로그램 삭제 3. 화이트 리스트에 포함 4. 무시\n");
		scanf("%d", &num);
		switch (num) {
			case 1://kill
				for (j = 0; j < numOfProcess; j++) {
						if(!strcmp(reportInfo[i].path,processInfo[i].path) && !strcmp(reportInfo[i].username,processInfo[i].username)) {
							kill(atoi(processInfo[i].pid), 9);
						}
					break;
				}
				break;
		case 2: //unlink
			if (unlink(reportInfo[i].path) == -1) {
				perror("fail to unlink!!");
				break;
			}
			break;
		case 3: //화이트 리스트에 포함
			strcpy(newWhiteList.path, reportInfo[i].path);
			strcpy(newWhiteList.username, reportInfo[i].username);
			writeWhiteList(&newWhiteList, 1, 1);
			break;
		case 4: //무시 
			break;
		}
		strcpy(newReport.path, reportInfo[i].path);
		strcpy(newReport.username, reportInfo[i].username);
		strcpy(newReport.detectedTime, reportInfo[i].detectedTime);
		deleteReport(&newReport, 1);
	}
	execl("./ProcessWiteList","./ProcessWhiteList", NULL);
}

