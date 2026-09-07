#ifndef PROC_INFO_H
#define PROC_INFO_H

#define PROC_PATH "/proc/%d/status"

enum states {
    R,
    S,
    D,
    T,
    t,
    Z,
    I,
};

struct proc_info {
    int pid;
    char* name;
    enum states state;
    int ppid;
    char* cmdline;
    int ofd;
};


struct proc_info get_info(int pid);

#endif /* PROC_INFO_H */

