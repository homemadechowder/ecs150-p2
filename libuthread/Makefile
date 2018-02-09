# Target library
lib := libuthread.a
CFLAGS := -Wall -Werror -g
all: $(lib) 


#uthread: queue.o uthread.o context.o 
#	gcc $(CFLAGS) $< -o $@

$(lib): queue.o uthread.o context.o
	ar rcs $(lib) $< -l

uthread.o: uthread.c context.h preempt.h queue.h uthread.h
	gcc $(CFLAGS) $< -c

context.o: context.c context.h preempt.h uthread.h
	gcc $(CFLAGS) $< -c

queue.o: queue.c queue.h
	gcc $(CFLAGS) $< -c


clean:
	rm -rf $(lib) queue.o queue
## TODO: Phase 1.1
