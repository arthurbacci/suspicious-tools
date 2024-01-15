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
/* #include <locale.h> */


#define ERRNO_MACRO(n, s) {(n), (s)},
static struct {
	int n;
	char *s;
} errnos_data[] = {
	#include "errnos.def"
	{0, NULL}
};
#undef ERRNO_MACRO

void usage() {
	fprintf(stderr, "errno [name-or-number]\n");
	exit(1);
}

void print_errno(size_t i) {
	printf(
		"%s %d %s\n",
		errnos_data[i].s, errnos_data[i].n, strerror(errnos_data[i].n)
	);
}

void print_all_errnos() {
	size_t i;
	for (i = 0; errnos_data[i].s; i++)
		print_errno(i);
}

void print_n_errno(int number) {
	size_t i;
	/* TODO: maybe take advantage of the list being 
	 * sorted? */

	for (i = 0; errnos_data[i].s; i++)
		if (errnos_data[i].n == number)
			print_errno(i);
}

void print_s_errno(char *s) {
	size_t i;
	/* break is fine since there are no duplicate numbers
	 * for a name. */
	for (i = 0; errnos_data[i].s; i++)
		if (0 == strcmp(errnos_data[i].s, s)) {
			print_errno(i);
			break;
		}
}

int main(int argc, char **argv) {
	/* setlocale(LC_ALL, ""); */


	if (argc == 1) {
		print_all_errnos();
	} else if (argc == 2) {
		size_t i;
		/* if argv[1][i] is NULL, all the characters are digits */
		for (i = 0; argv[1][i] && isdigit(argv[1][i]); i++);

		if (argv[1][i]) print_s_errno(argv[1]);
		else            print_n_errno(atoi(argv[1]));
	} else {
		usage();
	}


	return 0;
}


