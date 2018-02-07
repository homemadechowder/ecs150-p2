#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

/* TODO Phase 2 */

#define INIT 0
#define READY 1
#define RUNNING 2
#define FINISHED 3
#define WAITINIG 4


queue_t library; // it is a pointer to the queue 

typedef struct TCB {
	// contains pointer to top of stack: void* ss_sp 
	uthread_ctx_t *ctx;  
	uthread_t TID;
	int state; 
	int ret;
	bool joined; // to make sure if it was collected or not
};

struct TCB *curBlock; // current running block


int getRunningBlock(queue_t lib, void* block, void* arg)
{
	/*
	 * return 1 if we found current running process
	 * return 0 otherwise
	 */
	
	struct TCB *cur = (struct TCB *) block;
	
	if(cur->state == RUNNING)
	{
		curBlock = (struct TCB*) block;
		return 1;
	}

	return 0;
	
}

int findTID(queue_t lib, void* block, void* arg)
{

	struct TCB *cur = (struct TCB *) block;
	uthread_t tid = (uthread) arg;

	if(cur->TID == arg)
		return 1;

	return 0;

}



void uthread_yield(void)
{
	if(curBlock->state != RUNNING)
	{
		// should it do something
	}
	else 
	{
		
	}
	
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	void* arg;
	queue_func_t func = &getRunningBlock;
	// making block be the main thread
 	//curBlock = (struct TCB*) library->front->data;
  
	int iter = queue_iterate(library, func, arg, NULL);
	
	if(iter == 0)
		return curBlock->TID;

	return 0; // since it is a number don't know if it will work
		

	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	int create;
	struct TCB *tBlock = (struct TCB*)malloc(sizeof(struct TCB));
	if(library == NULL) // we are creating our main thread
	{
		// this is questionable ??????????
		queue_enqueu(library, tBlock);
		return 0; // TID of main is 0
	}
	
	void *stack = uthread_ctx_alloc_stack();
	create = uthread_ctx_init(tBlock->ctx, stack, func, arg);
	
	if (create == 0)
	{
		// enqueue new thread
		queue_enqueu(library, tBlock);
		// gets its TID
		tBlock->TID = library->length - 1;
		tBlokc->state = READY;
		tBlock->joined = false;
		// return TID 
		return tBlock->TID;
	} 
	
	if (create == -1)
		return -1;

	/* TODO Phase 2 */
}

void uthread_exit(int retval)
{
		if(curBlock->state != RUNNING)
		{
			uthread_self();
		}

		curBlock->ret = retval;
		curBlock->state = FINISHED;
}

int uthread_join(uthread_t tid, int *retval)
{
	queue_t func = &findTID;
	struct TCB * deadTCB;
	int iter = queue_iterate(library, func, (void*)&tid , &deadTCB);
	
	if( tid == uthread_self() || tid <= 0 || !uthread_self()
		||deadTCB->joined)
		return -1;
	
	// once the thread is joined raise flag to 

	while (1)
	{
		return 0;
	}

	return 0;
	/* TODO Phase 2 
	Execute an infinite loop in which:
	If there are no more threads which are ready to run in the
	system, break the loop and return
	Otherwise simply yield to next available thread
	*/
	/* TODO Phase 3 */
}

