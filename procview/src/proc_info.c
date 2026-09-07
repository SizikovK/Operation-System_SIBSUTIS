#include "../include/proc_info.h"
#include <stdio.h>

struct proc_info get_info(int pid) {
    char path[256];
    struct proc_info ret_info;

    snprintf(path, sizeof(path), PROC_PATH, pid);
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Процесс не существует");
        struct proc_info ret;
        return ret;
    }
    char line[256];
    int i = 0; 
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        switch (i)
        {
        case constant expression:
            /* code */
            break;
        
        default:
            break;
        }
    }
    fclose(file);    
    return ret_info;
}