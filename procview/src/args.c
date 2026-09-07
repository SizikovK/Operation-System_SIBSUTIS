#include "../include/args.h"
#include <stdlib.h>
#include <string.h>

struct proc_args set_args(int argc, char **argv) {
	struct proc_args a;
	a.pid = -1;
	if (argc >= 2) {
		/* try parse first arg as pid */
		char *end;
		long v = strtol(argv[1], &end, 10);
		if (end != argv[1] && *end == '\0' && v > 0) {
			a.pid = (int)v;
		}
	}
	return a;
}

