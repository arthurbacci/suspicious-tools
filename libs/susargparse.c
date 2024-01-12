#include <string.h>

#include "susargparse.h"

int susargparse_afterparse = 1;

int susargparse_find(int argc, char **argv, char *cmd) {
	int i, single = 0, r = -1;

	if (cmd[0] == '\0') return -1;
	single = cmd[1] == '\0';

	for (i = 0; i < argc; i++) {
		if (argv[i][0] != '-') continue;

		if (single && argv[i][1] != '-') {
			int j;
			for (j = 1; argv[i][j]; j++)
				if (argv[i][j] == *cmd) r = i;
		} else if (!single && argv[i][1] == '-') {
			if (0 == strcmp(&argv[i][2], cmd)) r = i;
		}
	}
	/* If we return from the loop, afterparse won't work correctly */
	
	return r;
}

int susargparse_option(int argc, char **argv, char *cmd) {

	int p = susargparse_find(argc, argv, cmd);
	if (p == -1) return 0;
	
	if (p + 1 > susargparse_afterparse) susargparse_afterparse = p + 1;
	
	return 1;
}

int susargparse_value(int argc, char **argv, char *cmd) {
	int p = susargparse_find(argc, argv, cmd);
	if (p == -1) return -1;

	if (p + 2 > susargparse_afterparse) susargparse_afterparse = p + 2;

	return p + 1;
}


