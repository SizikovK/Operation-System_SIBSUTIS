#ifndef ARGS_H
#define ARGS_H

struct proc_args {
    int pid;
};

struct proc_args set_args(int argc, char **argv);

#endif /* ARGS_H */
