/*
 * arthurbacci's suspicious tools
 * PART OF THE BUILD OF ERRNO
 *
 * Distributed under the public domain under the terms of either:
 * 	Unlicense
 * 	CC0
 * 	BSD0
 *
 * ~ Arthur Bacci 2024
 */

#include <stdio.h>
#include <errno.h>
#include <limits.h>

int main() {
	int max = INT_MIN;

	FILE *fp = fopen("errnos.def", "w");

	
	while (1) {
		int min = INT_MAX;

#define ERRNO_MACRO(n, s) if ((n) < min && (n) > max) min = n;
#include "errnos_draft.def"
#undef ERRNO_MACRO

		if (min == INT_MAX) break;
		max = min;

#define ERRNO_MACRO(n, s) if ((n) == min) fprintf(fp, "ERRNO_MACRO(%d, \"%s\")\n", (n), (s));
#include "errnos_draft.def"
#undef ERRNO_MACRO

	}

	fclose(fp);

	return 0;
}

