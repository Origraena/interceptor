#include "interceptor.h"

int popen2(const char* cmd, int* fd_in, int* fd_out) {
	printf("cmd + %s\n",cmd);
	int p_in[2];
	int p_out[2];
	int pid;
	if (pipe(p_out) < 0) {
		perror("p_out");
		return -1;
	}
	if (pipe(p_in) < 0) {
		perror("p_in");
		close(p_out[FD_READ]);
		close(p_out[FD_WRITE]);
		return -2;
	}
	pid = fork();
	if (pid < 0) {
		perror("fork: ");
		return -3;
	}
	if (pid == 0) {
		close(p_in[FD_WRITE]);
		close(p_out[FD_READ]);
		dup2(p_in[FD_READ],FD_STDIN);
		dup2(p_out[FD_WRITE],FD_STDOUT);
		execl(cmd,cmd,0);
		perror("execl");
		close(p_in[FD_READ]);
		close(p_out[FD_WRITE]);
		return -4;
	}
	else {
		if (fd_in == 0)
			close(p_in[FD_WRITE]);
		else
			*fd_in = p_in[FD_WRITE];
		if (fd_out == 0)
			close(p_out[FD_READ]);
		else
			*fd_out = p_out[FD_READ];
	}
	return pid;
}

int ptyopen(const char* cmd, int* fd) {
	int fd_master,fd_slave;
	int pid;
	if (openpty(&fd_master,&fd_slave,0,0,0) < 0) {
		perror("openpty");
		return -1;
	}
	pid = fork();
	if (pid < 0) {
		perror("fork");
		return -2;
	}
	if (pid == 0) {
		close(fd_master);
		close(0); close(1); close(2);
		dup(fd_slave); dup(fd_slave); dup(fd_slave);
		setsid();
		ioctl(0,TIOCSCTTY,1);
		execl(cmd,cmd,0);
		perror("execl");
		return -3;
	}
	else {
		close(fd_slave);
		*fd = fd_master;
	}
	return pid;
}

