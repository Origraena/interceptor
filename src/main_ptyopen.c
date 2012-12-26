#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include "interceptor.h"

int main(int argc, char** argv) {
	int fd_master;
	const char* cmd = "/usr/games/nethack";
	char buffer[2];
	buffer[0] = ' ';
	buffer[1] = '\0';
	char c;

	if (ptyopen(cmd,&fd_master) < 0) {
		perror("ptyopen");
		return -1;
	}

	if (setvbuf(stdin,&c,_IONBF,sizeof(char)) < 0) {
		perror("setvbuf"); 
		return -2;      // TODO clean
	}

	while (1) {
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(fd_master,&fds);
		FD_SET(FD_STDIN,&fds);
		if (select(fd_master+1,&fds,0,0,0) < 0) {
			perror("select");
			break;
		}

		if (FD_ISSET(fd_master,&fds)) {
			if (read(fd_master,buffer,sizeof(char)) < 0) {
				perror("read");
				break;
			}
			printf("%c\n",buffer[0]);
		}

		if (FD_ISSET(FD_STDIN,&fds)) {
/*			if (read(FD_STDIN,buffer,sizeof(char)) < 0) {
				perror("read");
				break;
			}
			if (write(fd_master,buffer,sizeof(char)) < 0) {
				perror("write");
				break;
			}*/
			if (read(FD_STDIN,&c,sizeof(char)) < 0) {
				perror("read");
				break;
			}
			if (write(fd_master,&c,sizeof(char)) < 0) {
				perror("write");
				break;
			}
		}

	}
	
	wait();
	close(fd_master);
	return 0;
}

