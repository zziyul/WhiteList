#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#include "details.h"
#include <signal.h>

int readProcess(struct pinfo* processInfo);
int readWhiteList(struct winfo* whiteListInfo);
int readReport(struct rinfo* reportInfo);
int writeWhiteList(struct winfo* info, int n, int option);
int writeReport(struct rinfo* info, int n, int option);
int deleteReport(struct rinfo* info, int n);

int main(/*int argc,  char *const argv[]*/){
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

	char str[20];
	pid_t pid;
	while(1) {
		printf("다음 중 하나를 선택하세요.\n");
		printf("1. 화이트리스트 관리\n2. 감시 시작\n3.리포트 확인\n");
		while(1) {
			scanf("%s", str);
			if ((atoi(str) != 1) && (atoi(str) != 2 && (atoi(str) != 3))) {
				printf("제대로 입력하세요.\n");
			}
			else break;
		}
		
		if (atoi(str) == 1) {
			printf("다음 중 하나를 선택하세요.\n");
			printf("1. 화이트리스트 초기화\n2.화이트리스트 보강\n");
			while(1) {
				scanf("%s", str);
				if ((atoi(str) != 1) && (atoi(str) != 2)) {	
					printf("제대로 입력하세요.\n");
				}
				else break;
			}
			pid=fork();
			if(pid==-1){
				perror("Failed to folk");
			}
			if (pid==0) {	
				if (atoi(str) == 1) {
					if (execl("./WhiteListEditor", "./WhiteListEditor", "0", NULL)==-1) {
						perror("execl에 실패했습니다.");
						return -1;
					}
				}
				else {
					if (execl("./WhiteListEditor", "./WhiteListEditor", "1", NULL)==-1) {
						perror("execl에 실패했습니다.");
						return -1;
					}
				}
			}
		}
		else if (atoi(str) == 2) {
			pid=fork();
			if(pid==-1){
				perror("Failed to folk");
			}
			if (pid==0) {
				if (execl("./ProcessInspector","./ProcessInspector", "1", NULL)==-1) {
					perror("execl에 실패했습니다.");
					return -1;
				}
			}
		}
		else {
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
							if(!strcmp(reportInfo[i].path,processInfo[j].path) && !strcmp(reportInfo[i].username,processInfo[j].username)) {
								fprintf(stderr, "%d\n", atoi(processInfo[j].pid));
								if (kill(atoi(processInfo[i].pid), 9) == -1) {
									perror("실패");
									
								}
								break;
							}
						}
						if (j == numOfProcess) {
							printf("해당 프로세스가 실행 중이 아닙니다.\n");
						}
						break;
					case 2: //unlink
						if (unlink(reportInfo[i].path) == -1) {
							perror("fail to unlink!!");
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
		}
		printf("프로그램을 종료하겠습니까?(y,n)\n");
		while(1) {
			scanf("%s",str);
			if (str[0] == 'y' || str[0] == 'Y') break;
			else if (str[0] != 'n' && str[0] != 'N') {
				printf("제대로 입력하세요.\n");
			}
			else break;
		}
		if (str[0] == 'y' || str[0] == 'Y') break;
	}
	return 0;
} 
