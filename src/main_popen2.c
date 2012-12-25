#include <stdlib.h>
#include <stdio.h>
#include "interceptor.h"

int main(int argc, char** argv) {
	int fd_in,fd_out;
	char reading[2];
	const char* cmd = "/bin/ls";
	reading[0] = ' ';
	reading[1] = '\0';
	if (popen2(cmd,&fd_in,&fd_out) < 0)
		return -1;
	while (reading[0] != EOF) {
		if (read(fd_out,reading,sizeof(char)) < 0) {
			perror("read");
			break;
		}
		printf("%c",reading[0]);
	}
	wait();
	close(fd_in);
	close(fd_out);
	return 0;
}

