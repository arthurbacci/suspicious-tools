/*
 * arthurbacci's suspicious tools
 * TIMEOUT
 *
 * Distributed under the public domain under the terms of either:
 * 	Unlicense
 * 	CC0
 * 	BSD0
 *
 * ~ Arthur Bacci 2024
 */

#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <susargparse.h>


void usage() {
	fprintf(stderr, "suspicious\n");
	exit(125);
}


int main(int argc, char **argv) {
	pid_t child;
	int interval = 0;

	if (susargparse_afterparse + 1 >= argc) usage();

	/* TODO: return error if not a number */
	interval = atoi(argv[susargparse_afterparse]);

	switch (child = fork()) {
		case -1:
			fprintf(stderr, "Couldn't fork\n");
			return 1;
		case 0:
			execvp(argv[susargparse_afterparse + 1], &argv[susargparse_afterparse + 1]);
			fprintf(stderr, "Couldn't execl: %s\n", strerror(errno));
			_exit(1);
	}

	for (; interval; interval--) {
		int status;
	
		if (waitpid(child, &status, WNOHANG) == child) {
			return status;
		}

		sleep(1);
	}

	if (kill(child, SIGTERM) == -1) {
		perror("kill");
		return 125;
	}

	return 124;
}

