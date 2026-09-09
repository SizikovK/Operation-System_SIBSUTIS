#include "args.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int parse_pid(int argc, char **argv, pid_t *pid) {
    char *end;
    long value;

    if (argc != 2) {
        return -1;
    }

    errno = 0;
    value = strtol(argv[1], &end, 10);
    if (errno == ERANGE || end == argv[1] || *end != '\0' ||
        value <= 0 || value > INT_MAX) {
        return -1;
    }

    *pid = (pid_t)value;
    return 0;
}
