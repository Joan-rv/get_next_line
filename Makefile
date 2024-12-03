CC=gcc
CFLAGS=-fPIC -Wall -Wextra -Werror -g -I.
AR=ar
RM=rm -f
OBJ=get_next_line.o
DESTDIR=/usr/local

.PHONY: clean all install

release: CFLAGS+=-O2
release: all

debug: CFLAGS+=-Werror -g
debug: all

all: test lib

install: lib
	install -m0755 libgnl.so $(DESTDIR)/lib/
	install -m0755 libgnl.h $(DESTDIR)/include/

test: test.o libgnl.a
	$(CC) -lm $^ -o $@

lib: libgnl.a libgnl.so

libgnl.a: $(OBJ)
	$(AR) rcs $@ $^

libgnl.so: $(OBJ)
	$(CC) -shared $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) *.o *.a *.so test
