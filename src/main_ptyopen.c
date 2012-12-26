#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "interceptor.h"

int main(int argc, char** argv) {
	int fd_master,fd_log;
	const char* cmd = "/usr/games/nethack";
	char c;
	int running = 1;

	if (ptyopen(cmd,&fd_master) < 0) {
		perror("ptyopen");
		return -1;
	}

	if (setvbuf(stdin,&c,_IONBF,sizeof(char)) < 0) {
		perror("setvbuf"); 
		return -2;      // TODO clean
	}

	// TODO think that if stdin first char is #
	// the command should be buffered

	fd_log = open("./ori.log",O_WRONLY|O_CREAT|O_TRUNC);
	if (fd_log < 0) {
		perror("log open");
		return -3;      // TODO clean
	}

	while (running) {
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(fd_master,&fds);
		FD_SET(FD_STDIN,&fds);
		if (select(fd_master+1,&fds,0,0,0) < 0) {
			perror("select");
			break;
		}

		if (FD_ISSET(fd_master,&fds)) {
			if (read(fd_master,&c,sizeof(char)) < 0) {
				perror("read");
				running = 0;
				break;
			}
			printf("%c\n",c);
			if (write(fd_log,&c,sizeof(char)) < 0) {
				perror("log write");
				running = 0;
				break;
			}
			if (write(fd_log,"\n",sizeof(char)) < 0) {
				perror("log write endline");
				running = 0;
				break;
			}
		}

		if (FD_ISSET(FD_STDIN,&fds)) {
			if (read(FD_STDIN,&c,sizeof(char)) < 0) {
				perror("read");
				running = 0;
				break;
			}
			if (c == 'Q') {   // TODO catch ^c signal
				running = 0;
//				write(fd_master,"#quit\nynnn",10);
				break;
			}
			if (write(fd_master,&c,sizeof(char)) < 0) {
				perror("write");
				running = 0;
				break;
			}
		}

	}
	
//	wait();
	close(fd_master);
	close(fd_log);
	return 0;
}

