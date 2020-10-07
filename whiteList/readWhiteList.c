#include<stdio.h>
// 파일 open을 위해 인클루드
#include<fcntl.h>
#include<sys/stat.h>
//파일읽기를 위해 인클루드
#include<unistd.h>

#include <errno.h>
#include <string.h>
#include "details.h"

int readWhiteList(struct winfo* whiteListInfo) {
	int k = 0;		// 읽은 정보 개수
	
	int whiteListFd;
	char buf[PATH_MAX];
	int numread = 0;
	int retval;

	if((whiteListFd = open("whitelist", O_RDONLY|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO)) == -1) {
		perror("Failed to open whitelist");
	}
        
	while(numread < PATH_MAX-1) {
		retval = read(whiteListFd, buf+numread, 1);
		if ((retval == -1) && (errno == EINTR)) continue;
		if (retval == 0) break;
		if (retval == -1) {
			perror("Failed to read whitelist");
			break;
		}
		numread++;
		if (buf[numread-1] == SEPERATOR) {
			buf[numread-1] = '\0';
			strcpy(whiteListInfo[k].path, buf);
			buf[0]='\0';
			numread = 0;
		}
		else if (buf[numread-1] == '\n') {
			buf[numread-1] = '\0';
			strcpy(whiteListInfo[k].username, buf);
			buf[0]='\0';
			numread = 0;
			k++;
		} 
	}

	while((close(whiteListFd) == -1) && (errno == EINTR));
	return k;
}
