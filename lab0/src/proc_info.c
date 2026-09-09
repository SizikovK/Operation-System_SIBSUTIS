#include "proc_info.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_SIZE 64
#define LINE_SIZE 512
#define CMD_CHUNK 256

static int make_path(char *path, size_t size, pid_t pid, const char *file) {
    int length = snprintf(path, size, "/proc/%d/%s", (int)pid, file);

    if (length < 0 || (size_t)length >= size) {
        fprintf(stderr, "Cannot create path for PID %d\n", (int)pid);
        return -1;
    }
    return 0;
}

static char *copy_string(const char *text) {
    size_t length = strlen(text) + 1;
    char *copy = malloc(length);

    if (copy != NULL) {
        memcpy(copy, text, length);
    }
    return copy;
}

static char *field_value(char *line) {
    char *value = strchr(line, ':');
    char *end;

    if (value == NULL) {
        return NULL;
    }

    value++;
    while (*value == ' ' || *value == '\t') {
        value++;
    }

    end = value + strlen(value);
    while (end > value && (end[-1] == '\n' || end[-1] == '\r' ||
                           end[-1] == ' ' || end[-1] == '\t')) {
        end--;
    }
    *end = '\0';
    return value;
}

static int read_status(struct proc_info *info, pid_t requested_pid) {
    char path[PATH_SIZE];
    char line[LINE_SIZE];
    int name_found = 0;
    int state_found = 0;
    int pid_found = 0;
    int ppid_found = 0;
    FILE *file;

    if (make_path(path, sizeof(path), requested_pid, "status") != 0) {
        return -1;
    }

    file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open %s: %s\n", path, strerror(errno));
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *value;

        if (strncmp(line, "Name:", 5) == 0) {
            value = field_value(line);
            info->name = copy_string(value);
            if (info->name == NULL) {
                fprintf(stderr, "Cannot allocate memory\n");
                fclose(file);
                return -1;
            }
            name_found = 1;
        } else if (strncmp(line, "State:", 6) == 0) {
            value = field_value(line);
            if (value != NULL && *value != '\0') {
                info->state = *value;
                state_found = 1;
            }
        } else if (strncmp(line, "Pid:", 4) == 0) {
            value = field_value(line);
            info->pid = (pid_t)strtol(value, NULL, 10);
            pid_found = 1;
        } else if (strncmp(line, "PPid:", 5) == 0) {
            value = field_value(line);
            info->parent_pid = (pid_t)strtol(value, NULL, 10);
            ppid_found = 1;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "Cannot read %s: %s\n", path, strerror(errno));
        fclose(file);
        return -1;
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Cannot close %s: %s\n", path, strerror(errno));
        return -1;
    }
    if (!name_found || !state_found || !pid_found || !ppid_found) {
        fprintf(stderr, "Required fields are missing in %s\n", path);
        return -1;
    }
    return 0;
}

static int read_cmdline(struct proc_info *info) {
    char path[PATH_SIZE];
    char chunk[CMD_CHUNK];
    char *buffer = NULL;
    size_t length = 0;
    FILE *file;

    if (make_path(path, sizeof(path), info->pid, "cmdline") != 0) {
        return -1;
    }
    file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open %s: %s\n", path, strerror(errno));
        return -1;
    }

    for (;;) {
        size_t count = fread(chunk, 1, sizeof(chunk), file);
        if (count > 0) {
            char *larger = realloc(buffer, length + count + 1);
            if (larger == NULL) {
                fprintf(stderr, "Cannot allocate memory\n");
                free(buffer);
                fclose(file);
                return -1;
            }
            buffer = larger;
            memcpy(buffer + length, chunk, count);
            length += count;
        }
        if (count < sizeof(chunk)) {
            break;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "Cannot read %s: %s\n", path, strerror(errno));
        free(buffer);
        fclose(file);
        return -1;
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Cannot close %s: %s\n", path, strerror(errno));
        free(buffer);
        return -1;
    }
    if (length == 0) {
        free(buffer);
        return 0;
    }

    for (size_t i = 0; i < length; i++) {
        if (buffer[i] == '\0') {
            buffer[i] = ' ';
        }
    }
    while (length > 0 && buffer[length - 1] == ' ') {
        length--;
    }
    buffer[length] = '\0';
    info->command_line = buffer;
    return 0;
}

static int count_file_descriptors(struct proc_info *info) {
    char path[PATH_SIZE];
    struct dirent *entry;
    DIR *directory;

    if (make_path(path, sizeof(path), info->pid, "fd") != 0) {
        return -1;
    }
    directory = opendir(path);
    if (directory == NULL) {
        fprintf(stderr, "Cannot open %s: %s\n", path, strerror(errno));
        return -1;
    }

    errno = 0;
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0) {
            info->fd_count++;
        }
    }

    if (errno != 0) {
        fprintf(stderr, "Cannot read %s: %s\n", path, strerror(errno));
        closedir(directory);
        return -1;
    }
    if (closedir(directory) != 0) {
        fprintf(stderr, "Cannot close %s: %s\n", path, strerror(errno));
        return -1;
    }
    return 0;
}

void process_info_init(struct proc_info *info) {
    info->pid = 0;
    info->parent_pid = 0;
    info->name = NULL;
    info->state = '?';
    info->command_line = NULL;
    info->fd_count = 0;
}

void process_info_destroy(struct proc_info *info) {
    free(info->name);
    free(info->command_line);
    process_info_init(info);
}

int process_info_load(struct proc_info *info, pid_t pid) {
    if (read_status(info, pid) != 0 ||
        read_cmdline(info) != 0 ||
        count_file_descriptors(info) != 0) {
        return -1;
    }
    return 0;
}
