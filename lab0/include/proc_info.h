#ifndef PROC_INFO_H
#define PROC_INFO_H

#include <stddef.h>
#include <sys/types.h>

struct proc_info {
    pid_t pid;
    pid_t parent_pid;
    char *name;
    char state;
    char *command_line;
    size_t fd_count;
};

void process_info_init(struct proc_info *info);
void process_info_destroy(struct proc_info *info);
int process_info_load(struct proc_info *info, pid_t pid);

#endif
