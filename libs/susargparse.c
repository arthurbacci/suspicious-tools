#include <string.h>

#include "susargparse.h"

int susargparse_option(int argc, char **argv, char *cmd) {
	int i, single = 0;
	
	if (cmd[0] == '\0') return 0;
	if (cmd[1] == '\0') single = 1;

	for (i = 0; i < argc; i++) {
		if (argv[i][0] != '-') continue;

		if (single && argv[i][1] != '-') {
			int j;
			for (j = 1; argv[i][j]; j++)
				if (argv[i][j] == *cmd) return 1;
		} else if (!single && argv[i][1] == '-') {
			if (0 == strcmp(&argv[i][2], cmd)) return 1;
		}
	}

	return 0;
}

int susargparse_value(int argc, char **argv, char *cmd) {
	int i, single = 0;

	if (cmd[0] == '\0') return 0;
	if (cmd[1] == '\0') single = 1;

	for (i = 0; i < argc; i++) {
		if (argv[i][0] != '-') continue;

		if (single && argv[i][1] != '-') {
			int l = strlen(argv[i]);
			if (argv[i][l - 1] == cmd[0]) return i + 1;
		} else if (!single && argv[i][1] == '-') {
			if (0 == strcmp(&argv[i][2], cmd)) return i + 1;
		}
	}

	return -1;
}

