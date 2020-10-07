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
		fprintf(stderr,"�߸��� ���ڸ� �Է��߽��ϴ�.");
	}
	return 0;
}
void ProcessInspector(int option) {
	struct pinfo processInfo[PROCESS_MAX];	// ���� ����ǰ� �ִ� ���μ��� ����
	struct winfo whitelistInfo[PROCESS_MAX];	// ȭ��Ʈ����Ʈ�� �ִ����μ��� ����
	struct rinfo reportInfo[PROCESS_MAX];	// ����Ʈ�� �ִ� ���μ��� ����
	
	struct rinfo newreport[PROCESS_MAX];

	int numOfProcess;	// �о�� ���η��� ����
	int numOfWhitelist;		// �о�� ȭ��Ʈ����Ʈ ����
	int numOfReport;	// �о�� ����Ʈ �׸� ����
	int numOfNew;		// ����Ʈ�� ������ �׸� ��

	time_t ctime;	// ���� �߰� �ð�
	char detectedTime[11];	// reportInfo�� ������ �ð�.

	int i, j, k;	// �ݺ���

	// ���� ������ �о�´�.
	numOfProcess = readProcess(processInfo);
	numOfWhitelist = readWhiteList(whitelistInfo);
	numOfReport = readReport(reportInfo);
	numOfNew = 0;
	
	// processInfo�� whitelistInfo�� ���Ѵ�.
	// processInfo���� ���μ��� ������ġ�� whitelistInfo���� ���μ��� ������ġ�� ���Ѵ�.
	for (i = 0; i < numOfProcess; i++) {
		for (j = 0; j < numOfWhitelist; j++) {
			// ���� ������ ���´ٸ� break
			if (!strcmp(processInfo[i].path, whitelistInfo[j].path) && !strcmp(processInfo[i].username, whitelistInfo[j].username)) {
				break;
			}
		}
		if (j != numOfWhitelist) continue;
		// �ɼ��� 1�̶�� report���� ���� �༮���� ���� ����
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

	//processInfo�� whitelistInfo�� �񱳰� ������ ProcessJudge�� �����Ų��.
	//pid_t pid;
	//pid = fork();
	//	if (pid == 0) {
	//	execl('ProcessJudge�� path', (char*)0);//ProcessJudge ����
	//}
}
