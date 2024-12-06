#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char* get_next_line(int fd) {
    static char* buf = NULL;
    static size_t size = 0;
    bool done_reading = false;
    while (true) {
        for (size_t i = 0; i * sizeof(char) < size; i++) {
            if (buf[i] == '\n') {
                return remove_as_string(&buf, &size, i);
            }
        }
        if (done_reading) {
            return remove_as_string(&buf, &size, size - 1);
        }
        buf = memresize(buf, size + BUFFER_SIZE);
        ssize_t read_size = read(fd, buf + size, BUFFER_SIZE);
        if (read_size < BUFFER_SIZE) {
            if (read_size < 0) {
                buf = memresize(buf, 0);
                size = 0;
                return NULL;
            }
            done_reading = true;
            buf = memresize(buf, size + read_size);
        }
        size += read_size;
    }
}
