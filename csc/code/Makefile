CC=gcc
CFLAGS=-I../include
DEPS = ../include/fibo.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
