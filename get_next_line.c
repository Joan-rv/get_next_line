#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUF_SIZE 4096

size_t min(size_t a, size_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

void* memresize(void* buf, size_t old_size, size_t size) {
    void* new_buf = NULL;
    if (size != 0) {
        new_buf = malloc(size);
        memcpy(new_buf, buf, min(size, old_size));
    }
    if (old_size != 0) {
        free(buf);
    }
    return new_buf;
}

char* remove_as_string(char** buf, size_t* size, size_t pos) {
    if (*size == 0) {
        return NULL;
    }
    char* tmp = malloc((pos + 2) * sizeof(char));
    memcpy(tmp, *buf, (pos + 1) * sizeof(char));
    tmp[pos + 1] = '\0';
    size_t old_size = *size;
    *size -= (pos + 1) * sizeof(char);
    memmove(*buf, *buf + (pos + 1), *size);
    *buf = memresize(*buf, old_size, *size);
    return tmp;
}

char* get_next_line(int fd) {
    static char* buf = NULL;
    static size_t size = 0;
    bool done_reading = false;
    char new[BUF_SIZE];
    while (true) {
        for (size_t i = 0; i * sizeof(char) < size; i++) {
            if (buf[i] == '\n') {
                return remove_as_string(&buf, &size, i);
            }
        }
        if (done_reading) {
            return remove_as_string(&buf, &size, size - 1);
        }
        ssize_t read_size = read(fd, new, BUFFER_SIZE);
        if (read_size < BUFFER_SIZE) {
            if (read_size < 0) {
                return NULL;
            }
            done_reading = true;
        }
        buf = memresize(buf, size, size + read_size);
        memcpy(buf + size / sizeof(char), new, read_size);
        size += read_size;
    }
}
