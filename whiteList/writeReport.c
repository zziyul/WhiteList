#include <unistd.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
#include "details.h"

int writeReport(struct rinfo* info, int n, int option) {
	char seperator[2];
	int fd;
	int i;

	seperator[0] = SEPERATOR;
	seperator[1] = '\0';
	if (option == 0) {
		if ((fd = open("report", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO)) == -1) {
			perror("Failed to open report");
			return -1;
		}
	}
	else if (option == 1) {
		if ((fd = open("report", O_RDWR|O_CREAT|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO)) == -1) {
			perror("Failed to open report");
			return -1;
		}
	}
	else {
		fprintf(stderr, "들어올 수 없는 옵션(옵션: %d)\n", option);
		return -1;
	}
	for (i = 0; i < n; i++) {
		write(fd, info[i].path, strlen(info[i].path));
		write(fd, seperator, 1);
		write(fd, info[i].username, strlen(info[i].username));
		write(fd, seperator, 1);
		write(fd, info[i].detectedTime, strlen(info[i].detectedTime));
		write(fd, "\n", 1);
	}
	return 0;
} 
