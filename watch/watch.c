/*
 * arthurbacci's suspicious tools
 * WATCH
 *
 * Distributed under the public domain under the terms of either:
 * 	Unlicense
 * 	CC0
 * 	BSD0
 *
 * ~ Arthur Bacci 2024
 */

/*
 * TODO: implement --precise
 * TODO: handle signals
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#include <susargparse.h>


#define OPT_NO_BAR     0x01
#define OPT_PRECISE    0x02
#define OPT_EXEC       0x04

static unsigned int options = 0;

void usage() {
	/* FIXME: less suspicious */
	fprintf(stderr, "amongus\n");
	exit(EXIT_FAILURE);
}

int parse_interval(const char *ss, struct timespec *interval) {
	char s[32];
	char *afterpoint = NULL;
	size_t i, afterpoint_len;

	strncpy(s, ss, 32);
	s[31] = '\0';

	for (i = 0; s[i]; i++) {
		if (s[i] == '.') {
			if (afterpoint == NULL) {
				afterpoint = &s[i + 1];
				s[i] = '\0';
			} else {
				return -1;
			}
		} else if (!isdigit(s[i])) {
			return -1;
		}
	}

	interval->tv_sec = strlen(s) ? atoi(s) : 0;
	interval->tv_nsec = afterpoint ? atoi(afterpoint) : 0;

	if (afterpoint == NULL) return 0;

	afterpoint_len = strlen(afterpoint);
	if (afterpoint_len > 9) return -1;
	for (i = afterpoint_len; i < 9; i++)
		interval->tv_nsec *= 10;

	return 0;
}

int main(int argc, char **argv) {
	int interval_i;
	char *command = NULL;
	struct timespec interval;

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

	if (interval_i == -1) {
		interval.tv_sec = 2;
		interval.tv_nsec = 0;
	} else {
		if (-1 == parse_interval(argv[interval_i], &interval))
			usage();
	}


	if (susargparse_afterparse >= argc) usage();

	if (!(options & OPT_EXEC) || !(options & OPT_NO_BAR)) {
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
		struct timeval before, after;
		struct timespec precise_interval;

		/* NOTE: may be soon deprecated
		 * It's being used because of the easy conversion to timespec
		 */
		if (options & OPT_PRECISE) gettimeofday(&before, NULL);

		printf("\x1b[1;1H\x1B[J\x1b[1;1H");

		if (!(options & OPT_NO_BAR)) {
			printf("\x1b[7m");
			if (options & OPT_EXEC) {
				printf("p>%s", command);
			} else {
				printf("sh>%s", command);
			}
			printf(" %d.%ds", (int)interval.tv_sec, (int)interval.tv_nsec);
			printf("\x1b[m\n");
		} else {
			fflush(stdout);
		}

		switch (child = fork()) {
			case -1:
				fprintf(stderr, "Couldn't fork\n");
				return 1;
			case 0:
				if (options & OPT_EXEC)
					execvp(argv[susargparse_afterparse], &argv[susargparse_afterparse]);
				else
					execl("/bin/sh", "sh", "-c", command, (char *) NULL);
				fprintf(stderr, "Couldn't execl: %s\n", strerror(errno));
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
		

		fflush(stdout);

		if (options & OPT_PRECISE) {
			gettimeofday(&after, NULL);
			after.tv_sec -= before.tv_sec;
			if (after.tv_usec < before.tv_usec) {
				after.tv_sec -= 1;
				after.tv_usec += 1000000;
			}
			after.tv_usec -= before.tv_usec;

			precise_interval.tv_sec = interval.tv_sec - after.tv_sec;
			precise_interval.tv_nsec = interval.tv_nsec;
			if (precise_interval.tv_nsec < 1000*after.tv_usec) {
				precise_interval.tv_sec -= 1;
				precise_interval.tv_nsec += 1000000000;
			}
			precise_interval.tv_nsec -= 1000*after.tv_usec;
			
			nanosleep(&precise_interval, NULL);
		} else {
			nanosleep(&interval, NULL);
		}
	}

	return 0;
}

