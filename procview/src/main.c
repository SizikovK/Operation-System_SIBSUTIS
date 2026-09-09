#include <stdio.h>

#include "args.h"
#include "output.h"
#include "proc_info.h"

int main(int argc, char **argv) {
    pid_t pid;
    struct proc_info info;

    if (parse_pid(argc, argv, &pid) != 0) {
        if (argc == 2) {
            fprintf(stderr, "Invalid PID: %s\n", argv[1]);
        }
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return 2;
    }

    process_info_init(&info);
    if (process_info_load(&info, pid) != 0) {
        process_info_destroy(&info);
        return 1;
    }

    print_info(&info);
    process_info_destroy(&info);
    return 0;
}
