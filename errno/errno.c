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
 * TODO: exit with error if the name-or-number isn't found.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#include <susargparse.h>

/* Maximum length of each line returned by `locale -a` */
#define LOCALE_BUFLEN 128


#define ERRNO_MACRO(n, s) {(n), (s)},
static struct {
	int n;
	char *s;
} errnos_data[] = {
	#include "errnos.def"
	{0, NULL}
};
#undef ERRNO_MACRO

void die(char *s) {
	fprintf(stderr, s);
	fprintf(stderr, "%d\n", errno);
	exit(1);
}

void print_errno(size_t i) {
	printf("%s %d %s\n",
	       errnos_data[i].s, errnos_data[i].n, strerror(errnos_data[i].n));
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

void find_errno(char *s) {
	size_t i;
	/* if s[i] is NULL, all the characters are digits */
	for (i = 0; s[i] && isdigit(s[i]); i++);

	if (s[i]) print_s_errno(s);
	else      print_n_errno(atoi(s));
}

void do_errno(int argc, char **argv) {
	if (susargparse_afterparse >= argc)
		print_all_errnos();
	else
		find_errno(argv[1]);
}

void do_all_locales(int argc, char **argv) {
	/* Similar to popen("locale -a"), but without using the shell.
	 * Child execs the command, using the pipe as stdout */

	int pipe_f[2];

	/* TODO: error handling */
	pipe(pipe_f);

	switch (fork()) {
	case -1:
		die("Couldn't fork\n");
		break;
	case 0:
		/* In case me (suspicious' `errno`) is called from a execve or
		 * something else that doesn't an stdout set by default
		 *
		 * Practise recommended by The Linux Programming Interface */
		if (pipe_f[1] != STDOUT_FILENO) {
			dup2(pipe_f[1], STDOUT_FILENO);
			close(pipe_f[1]);
		}
		execlp("locale", "locale", "-a", (char *) NULL);
		_exit(1);
	}
	/* I'm the parent */

	close(pipe_f[1]);

	/* `locale -a` returns one locale name by line */
	for (;;) {
		char buf[LOCALE_BUFLEN];
		size_t len;

		for (len = 0; 1 == read(pipe_f[0], &buf[len], 1); len++) {
			if (buf[len] == '\n') break;
			if (len >= LOCALE_BUFLEN) die("locale -a returned >= LOCALE_BUFLEN");
		}
		buf[len] = '\0';
		
		if (len == 0) break;
		

		setlocale(LC_ALL, buf);
		
		do_errno(argc, argv);
	}
}

int main(int argc, char **argv) {
	int all_locales;

	setlocale(LC_ALL, "");

	all_locales = susargparse_option(argc, argv, "S");
	
	if (all_locales) do_all_locales(argc, argv);
	else             do_errno(argc, argv);


	return 0;
}


