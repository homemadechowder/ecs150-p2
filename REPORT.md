ECS 150 Operating Systems: Project 2
## Overview
The purpose of this project is to investigate the way operating systems and 
high level applications utilize threads to abstract, manifest, and manage 
concurrency. In particular, we design a POSIX-style, user level thread 
library and its associated application programming interface. 


## Works Cited
In addition to the slides provided in 06.processes-threads.pdf, *Operating
Systems: Principles and Practice* chapter four, the assignment specifications, 
and assignment reference material, the following sources were used:

 - To assist in a singularly linked list implementation of a queue:
 https://www.geeksforgeeks.org/queue-set-2-linked-list-implementation/
 
 - To assist in the comprehension and use of queue.h's typedef function 
pointer:
https://stackoverflow.com/questions/4295432/typedef-function-pointer
 - To assist in the comprehension and use of ucontext.h:
 http://pubs.opengroup.org/onlinepubs/7908799/xsh/ucontext.h.html

 - To assist in debugging:
 http://valgrind.org/docs/manual/quick-start.html

 - To assist in using signals and timers in preempt.c:
 http://www.informit.com/articles/article.aspx?p=23618&seqNum=14
 
 - To brush up on the C programming language: 
 Forouzan, Behrouz A, and Richard F Gilberg. *Computer Science: A Structured
 Approach Using C*. 3rd ed., Thomas Course Technology, 2007.

## Testing
Several measures were taken to ensure faithfulness to the queue.c, uthread.c,
and preempt.c specifications. They are the following:

 - Using a small set of ten integers to test the basic functionality of our queue.
 - Using the functions provided in 3.2.3 1.3 to test queue_iterate().
 - Comparison with *OSPP*, Chapter Four, Figuers 4.6 and 4.7.
 - Analysis of the gdb output of uthread_hello.x at varying states of our 
 program.  
 - Analysis of the gdb output of uthread_yield.x at varying states of our 
 program.

## Implementation

### Phase 1:
The primary design choice of the first phase involved the underlying data
structure which would be used to implement the queue, which in turn 
would serve as our library. We concluded that a singularly linked list would
be sufficient for our purpose, as that data structure permits O(1) insertion 
and deletion (in the sense of the dequeue operation) and avoids the 
unnecessary added complexity of a doubly linked list. The queue struct 
itself featured pointers to the head and tail, as well as an int for length
tracking to facilitate the aforementioned time complexity in the queue's 
other functions. Perhaps trivially, we note that the node struct features a
void pointer data variable and a pointer to the next node.
 
### Phase 2:
In Phase 2, our central data structure was our Thread Control Block, or TCB. 
We found that the TCB was an ever evolving data structure which continually 
grew in features to meet our needs. At maturity, it contains a uthread_ctx_t
pointer for use in context switches, a uthread_t to keep track of its own TID
, an int ret to contain the value exit() receives as an argument, an int pointer 
retContain to house the pointer the thread uses to receive the address of the 
ret another thread receives in its exit, and two more of type uthread_t to keep 
track of which threads can been claimed for joining and which had made a 
claim to join. Because our implementation uses just one queue to house all 
threads, we also include state member variable to track which stage of the
thread life cycle any given thread is in. We have a global TCB to keep track
of the current running block.

We implemented our functions by having one queue (library) to hold all of 
the TCB's. In order to implement the required functions, we had helper 
functions to be used by queue_iterate() to traverse through our queue and 
retrieve the correct block. For example, to find a specific TID, to find the 
next ready block, and to find the current running block in case the global 
block was not up-to-date. When we implemented exit() we left our global 
block to be with a status of finished while it was being updated to be the 
next running block.

### Phase 3:
In order to implement the proper join function, we added the integers 
collected andcollecting to our struct to determine if the thread had been 
claimed to be joined or it was claiming a thread to join. At this phase we also
added the container for the return value received from exit() since we 
needed to save the address the retval was to be stored. We adjusted the 
yield function since now join also called yield while our global block had a status
of waiting instead of running. 

### Phase 4:
Work on the final phase was highly dependent on researching signals and timers, 
constructs with which my partne and I had had little exposure up until then. 
Following examples from the provide reference material, setitimer is used to send 
SIGVTALRMS after and initial delay of 100,000 microseconds and then every 100,000
microseonds afterwards. A signal handler function then calls uthread_yield() to
interrupt a thread.  
