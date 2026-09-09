#include "output.h"

#include <stdio.h>

static const char *state_description(char state) {
    switch (state) {
        case 'R': return "running";
        case 'S': return "sleeping";
        case 'D': return "uninterruptible sleep";
        case 'T': return "stopped";
        case 't': return "tracing stop";
        case 'Z': return "zombie";
        case 'I': return "idle";
        default:  return "unknown";
    }
}

void print_info(const struct proc_info *info) {
    printf("Process information\n\n");
    printf("PID: %d\n", (int)info->pid);
    printf("Name: %s\n", info->name);
    printf("State: %c (%s)\n", info->state,
           state_description(info->state));
    printf("Parent PID: %d\n", (int)info->parent_pid);
    printf("Command line: %s\n",
           info->command_line != NULL ? info->command_line : "[not available]");
    printf("Open file descriptors: %zu\n", info->fd_count);
}
