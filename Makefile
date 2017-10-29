sources = csvread.c mergesort.c navigator.c print.c sorter.c
headers = sorter.h mergesort.h
objects = $(patsubst %.c, %.o, $(sources))

sorter: $(objects)
	$(CC) -lm $(LDFLAGS) -o sorter $(objects)

$(objects): %.o: %.c $(headers)
	$(CC) -c $(CPPFLAGS) -Wall -g -O2 $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rvf sorter $(objects)
