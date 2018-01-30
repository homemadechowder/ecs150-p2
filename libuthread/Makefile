# Target library
lib := libuthread.a
CFLAGS := -Wall -Werror
all: $(lib) queue

$(lib): queue.o
	ar rcs $(lib) $^

queue: queue.o
	gcc $(CFLAGS) -c -o $@  $^

#$(lib): queue.o
#	ar rcs $(lib) $^

queue.o: queue.c queue.h
	gcc $(CFLAGS) -c -o $@  $<


clean:
	rm -rf $(lib) queue.o queue
## TODO: Phase 1.1
