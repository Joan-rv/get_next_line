#include <fcntl.h>
#include <libgnl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }
    int fd;
    if (strcmp(argv[1], "stdin") == 0) {
        fd = 0;
    } else {
        fd = open(argv[1], O_RDONLY);
    }
    char* line;
    while ((line = get_next_line(fd))) {
        printf("%s", line);
        fflush(stdout);
        free(line);
    }
    close(fd);
    return 0;
}
