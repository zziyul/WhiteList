#include "details.h"
#include <stdio.h>
#include <string.h>

int readReport(struct rinfo* reportInfo);
int writeReport(struct rinfo* info, int n, int option);

int deleteReport(struct rinfo* deleteInfo, int n) {
	struct rinfo reportInfo[PROCESS_MAX];
	int numOfReport;
	int i, j;	// 반복자	
	
	numOfReport = readReport(reportInfo);
	for (i=0;i<n;i++) {
		for(j=0;j<numOfReport;j++) {
			if (!strcmp(deleteInfo[i].path, reportInfo[j].path) && !strcmp(deleteInfo[i].username, reportInfo[j].username) && !strcmp(deleteInfo[i].detectedTime, reportInfo[j].detectedTime)) {
				strcpy(reportInfo[i].path, reportInfo[numOfReport-1].path);
				strcpy(reportInfo[i].username, reportInfo[numOfReport-1].username);
				strcpy(reportInfo[i].detectedTime, reportInfo[numOfReport-1].detectedTime);
				numOfReport--;
				break;
			}
		}
	}
	if (writeReport(reportInfo, numOfReport, 0) == -1) {
		fprintf(stderr, "리포트를 쓰는 중 오류가 발생했습니다.\n");
		return -1;
	}
	return 0;
}
