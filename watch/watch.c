#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include <susargparse.h>

#define OPT_NO_BAR     0x01
#define OPT_PRECISE    0x02
#define OPT_EXEC       0x04

static unsigned int options = 0;

void usage() {
	fprintf(stderr, "amongus\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	int interval_i, interval;
	char *command = NULL;

	/* TODO: consider using getopt */

	options |= OPT_NO_BAR  * susargparse_option(argc, argv, "t");
	options |= OPT_NO_BAR  * susargparse_option(argc, argv, "no-title");
	options |= OPT_PRECISE * susargparse_option(argc, argv, "p");
	options |= OPT_PRECISE * susargparse_option(argc, argv, "precise");
	options |= OPT_EXEC    * susargparse_option(argc, argv, "x");
	options |= OPT_EXEC    * susargparse_option(argc, argv, "exec");

	if (-1 == (interval_i = susargparse_value(argc, argv, "n")))
		interval_i = susargparse_value(argc, argv, "interval");
	
	if (interval_i >= argc) usage();

	if (interval_i == -1) interval = 2;
	else interval = atoi(argv[interval_i]);

	if (susargparse_afterparse >= argc) usage();

	if (!(options & OPT_EXEC)) {
		size_t len = 0;
		int i;
		char *cmd;

		for (i = susargparse_afterparse; i < argc; i++) {
			len += strlen(argv[i]);
			len++;
		}

		command = malloc(len * sizeof(char));
		cmd = command;

		for (i = susargparse_afterparse; i < argc; i++) {
			strcpy(cmd, argv[i]);
			cmd += strlen(argv[i]);
			*cmd = ' ';
			cmd++;
		}
		cmd--;
		*cmd = '\0';
	}
	

	while (1) {
		pid_t child;
		int waitpid_status;

		printf("\x1b[1;1H\x1B[J\x1b[1;1H");

		if (!(options & OPT_NO_BAR))
			printf("\x1b[7mEvery %d seconds\x1b[m\n", interval);
		else
			fflush(stdout);

		switch (child = fork()) {
			case -1:
				fprintf(stderr, "Couldn't fork\n");
				return 1;
			case 0:
				if (options & OPT_EXEC)
					execvp(argv[susargparse_afterparse], &argv[susargparse_afterparse]);
				else
					execl("/bin/sh", "sh", "-c", command, (char *) NULL);
				fprintf(stderr, "Couldn't execl %s\n", strerror(errno));
				_exit(1);
			default:
				if (waitpid(child, &waitpid_status, 0) == -1) {
					fprintf(stderr, "Couldn't watch my children die\n");
					return 1;
				}/* else {
					if (waitpid_status != 0)
						beeeeep()
				}*/
		}

		if (options & OPT_EXEC) {
			
		}
		

		fflush(stdout);

		sleep(interval);
	}

	return 0;
}

