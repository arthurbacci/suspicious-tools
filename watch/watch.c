#include <stdio.h>
#include <stdlib.h>

#include <susargparse.h>

#define OPT_NO_BAR     0x01
#define OPT_PRECISE    0x02

static unsigned int options = 0;

void usage() {
	fprintf(stderr, "amongus\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	int interval;

	options |= OPT_NO_BAR * susargparse_option(argc, argv, "t");
	options |= OPT_NO_BAR * susargparse_option(argc, argv, "no-title");
	options |= OPT_PRECISE * susargparse_option(argc, argv, "p");
	options |= OPT_PRECISE * susargparse_option(argc, argv, "precise");

	printf("%u\n", options);

	if (-1 == (interval = susargparse_value(argc, argv, "n")))
		interval = susargparse_value(argc, argv, "interval");
	
	if (interval == -1 || interval >= argc)
		usage();

	
	printf("%s\n", argv[interval]);

	return 0;
}

