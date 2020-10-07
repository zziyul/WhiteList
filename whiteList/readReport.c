#include<stdio.h>
// 파일 open을 위해 인클루드
#include<fcntl.h>
#include<sys/stat.h>
//파일읽기를 위해 인클루드
#include<unistd.h>

#include <errno.h>
#include <string.h>
#include "details.h"

int readReport(struct rinfo* reportInfo) {
	int k = 0;	// 읽은 파일의 개수
	
	int reportFd;
	char buf[PATH_MAX];
	int numread = 0;
	int retval;
	int count = 0;	// path에 저장할지 detectedTime에 저장할지 구분하기 위한 변수

	if((reportFd = open("report", O_CREAT|O_RDONLY, S_IRWXU|S_IRWXG|S_IRWXO)) == -1) {
		perror("Failed to open whitelist");
	}
        
	while(numread < PATH_MAX-1) {
		retval = read(reportFd, buf+numread, 1);
		if ((retval == -1) && (errno == EINTR)) continue;
		if (retval == 0) break;
		if (retval == -1) {
			perror("Failed to read report");
			break;
		}
		numread++;
		if (buf[numread-1] == SEPERATOR) {
			buf[numread-1] = '\0';
			if (count%2 == 0) {
				strcpy(reportInfo[k].path, buf);
			}
			else {
				strcpy(reportInfo[k].username, buf);
			}
			buf[0]='\0';
			numread = 0;
			count++;
		}
		else if (buf[numread-1] == '\n') {
			buf[numread-1] = '\0';
			strcpy(reportInfo[k].detectedTime, buf);
			k++;
			buf[0]='\0';
			numread = 0;
		} 
	}

	while((close(reportFd) == -1) && (errno == EINTR));
	return k;
}
