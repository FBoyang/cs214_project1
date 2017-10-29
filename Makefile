sources = csvread.c mergesort.c navigator.c print.c sorter.c
headers = sorter.h mergesort.h
objects = $(patsubst %.c, %.o, $(sources))
CC = gcc

sorter: $(objects)
	$(CC) -lm $(LDFLAGS) -o sorter $(objects)

$(objects): %.o: %.c
	$(CC) -c $(CPPFLAGS) -Wall -g -O2 $(CFLAGS) $<

navigator.o sorter.o: sorter.h
sorter.o csvread.o mergesort.o print.o: mergesort.h


.PHONY: clean
clean:
	rm -rvf sorter $(objects)
