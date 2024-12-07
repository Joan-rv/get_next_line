#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

size_t min(size_t a, size_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

void* memresize(void* buf, size_t size) {
    if (size == 0) {
        free(buf);
        return NULL;
    }
    return realloc(buf, size);
}

char* remove_as_string(char** buf, size_t* size, size_t pos) {
    if (*size == 0) {
        return NULL;
    }
    char* tmp = malloc((pos + 2) * sizeof(char));
    memcpy(tmp, *buf, (pos + 1) * sizeof(char));
    tmp[pos + 1] = '\0';
    *size -= (pos + 1) * sizeof(char);
    memmove(*buf, *buf + (pos + 1), *size);
    *buf = memresize(*buf, *size);
    return tmp;
}

void init_static(char*** buf, size_t** size, rlim_t* fd_max) {
    struct rlimit rlp;
    getrlimit(RLIMIT_NOFILE, &rlp);
    *fd_max = rlp.rlim_cur;
    *buf = calloc(*fd_max, sizeof(char*));
    *size = calloc(*fd_max, sizeof(size_t));
}

char* get_next_line(int fd) {
    static char** buf = NULL;
    static size_t* size = NULL;
    static rlim_t fd_max;

    if (buf == NULL) {
        init_static(&buf, &size, &fd_max);
    }
    if (fd < 0 || fd_max < (rlim_t)fd) {
        return NULL;
    }

    bool done_reading = false;
    while (true) {
        for (size_t i = 0; i * sizeof(char) < size[fd]; i++) {
            if (buf[fd][i] == '\n') {
                return remove_as_string(&buf[fd], &size[fd], i);
            }
        }
        if (done_reading) {
            return remove_as_string(&buf[fd], &size[fd], size[fd] - 1);
        }
        buf[fd] = memresize(buf[fd], size[fd] + BUFFER_SIZE);
        ssize_t read_size = read(fd, buf[fd] + size[fd], BUFFER_SIZE);
        if (read_size < BUFFER_SIZE) {
            if (read_size < 0) {
                buf[fd] = memresize(buf[fd], 0);
                size[fd] = 0;
                return NULL;
            }
            done_reading = true;
            buf[fd] = memresize(buf[fd], size[fd] + read_size);
        }
        size[fd] += read_size;
    }
}
