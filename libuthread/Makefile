# Target library
lib := libuthread.a
CFLAGS := -Wall -Werror -g
all: $(lib) queue


queue: queue.o
	gcc $(CFLAGS) $< -o $@

$(lib): queue.o
	ar rcs $(lib) $^ -l

queue.o: queue.c queue.h
	gcc $(CFLAGS) $< -c


clean:
	rm -rf $(lib) queue.o queue
## TODO: Phase 1.1
