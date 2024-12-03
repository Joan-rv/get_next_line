NAME=libgnl
CC=gcc
CFLAGS=-fPIC -Wall -Wextra -Werror -g -I.
AR=ar
RM=rm -f
OBJ=get_next_line.o
DESTDIR=/usr/local

.PHONY: clean release debug all lib install

release: CFLAGS+=-O2
release: all

debug: CFLAGS+=-Werror -g
debug: all

all: test lib

install: lib
	install -m0755 libgnl.so $(DESTDIR)/lib/
	install -m0755 libgnl.h $(DESTDIR)/include/

test: test.o $(NAME).a
	$(CC) -lm $^ -o $@

lib: $(NAME).a $(NAME).so

$(NAME).a: $(OBJ)
	$(AR) rcs $@ $^

$(NAME).so: $(OBJ)
	$(CC) -shared $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) *.o *.a *.so test
