#ifndef INTERCEPTOR_H_
#define INTERCEPTOR_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pty.h>

#define FD_READ 0
#define FD_WRITE 1
#define FD_STDIN 0
#define FD_STDOUT 1

/**
 * Opens two pipes between the specified command and the main
 * process.
 * \param fd_in will be affected a writing end if not null.
 * \param fd_out will be affected a reading end if not null.
 */
int popen2(const char* cmd, int* fd_in, int* fd_out);

/**
 * Opens a pseudo terminal device.
 * \param fd will be affected to the master side, ie can be used
 * both for writing and reading to and from the command.
 */
int ptyopen(const char* cmd, int* fd);

#ifdef __cplusplus
	}
#endif

#endif // INTERCEPTOR_H_

