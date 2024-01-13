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
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

int main() {

#define ERRNO_MACRO(n, s) printf("%s %d\n", (s), (n));

#include "errnos.def"


	return 0;
}


