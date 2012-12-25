#include <stdlib.h>
#include <stdio.h>
#include "interceptor.h"

int main(int argc, char** argv) {
	int fd_master;
	const char* cmd = "/usr/games/nethack";
	char buffer[2];
	buffer[0] = ' ';
	buffer[1] = '\0';

	if (ptyopen(cmd,&fd_master) < 0) {
		perror("ptyopen");
		return -1;
	}

	while (1) {
		write(fd_master,"y",1); // tells nethack to select for you
		if (read(fd_master,buffer,1) < 0) {
			perror("read");
			break;
		}
		printf("%c\n",buffer[0]);
	}
	
	wait();
	close(fd_master);
	return 0;
}

