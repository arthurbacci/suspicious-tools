/*
 * arthurbacci's suspicious tools
 * ERRNO
 *
 * Distributed under the public domain under the terms of either:
 * 	Unlicense
 * 	CC0
 * 	BSD0
 *
 * ~ Arthur Bacci 2024
 */

/*
 * TODO: try to implement locales: strerror_l? maybe try to implement -S for
 *       listing all  locales, similarly to moreutils
 * TODO: maybe throw the data to a variable (static) to decrease binary size
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

void usage() {
	fprintf(stderr, "errno [name-or-number]\n");
	exit(1);
}

int main(int argc, char **argv) {
	if (argc == 1) {
#define ERRNO_MACRO(n, s) printf("%s %d %s\n", (s), (n), strerror(n));
#include "errnos.def"
#undef ERRNO_MACRO
	} else if (argc == 2) {
		/* boolean */
		int is_number = 1;
		size_t i;
		for (i = 0; argv[1][i]; i++) {
			if (!isdigit(argv[1][i])) {
				is_number = 0;
				break;
			}
		}

		if (is_number) {
			int number = atoi(argv[1]);
			/* SWITCH is not much of an option since there are
			 * duplicate numbers. Neither is else if. */
#define ERRNO_MACRO(n, s) if ((n) == number) printf("%s %d %s\n", (s), (n), strerror(n));
#include "errnos.def"
#undef ERRNO_MACRO
		} else {
			/* else if is fine since there are no duplicate numbers
			 * for a name. */
			if (0) {}
#define ERRNO_MACRO(n, s) else if (0 == strcmp((s), argv[1])) printf("%s %d %s\n", (s), (n), strerror(n));
#include "errnos.def"
#undef ERRNO_MACRO
		}
	} else {
		usage();
	}


	return 0;
}


