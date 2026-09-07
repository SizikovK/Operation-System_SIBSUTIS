#include <stdio.h>
#include "../include/args.h"
#include "../include/proc_info.h"
#include "../include/output.h"

int main(int argc, char **argv) {
    struct proc_args a = set_args(argc, argv);

    if (a.pid <= 0) {
        printf("Usage: %s <pid>\n", argc > 0 ? argv[0] : "procview");
        return 1;
    }

    struct proc_info p = get_info(a.pid);
    print_info(p);
    return 0;
}
