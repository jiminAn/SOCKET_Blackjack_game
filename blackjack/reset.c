#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	int fd;

	// 파일 열기
	fd = open("log.txt", O_TRUNC | O_CREAT, 0644);
	if (fd == -1) {
		perror("Open");
		exit(1);
	}

	close(fd);
}
