CC?=clang
CFLAGS?=-Wall -Wextra -O2

get_next_line: get_next_line.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm get_next_line
