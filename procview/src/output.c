#include "../include/output.h"
#include <stdio.h>

void print_info(struct proc_info p) {
    printf("PID: %d\n", p.pid);
    if (p.name) printf("Name: %s\n", p.name);
    printf("State: %d\n", p.state);
    printf("PPID: %d\n", p.ppid);
    if (p.cmdline) printf("Cmd: %s\n", p.cmdline);
    printf("Open fds: %d\n", p.ofd);
}
