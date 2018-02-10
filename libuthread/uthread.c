#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

/* TODO Phase 2 */

#define INIT 0
#define READY 1
#define RUNNING 2
#define FINISHED 3
#define WAITING 4


queue_t library; // it is a pointer to the queue 

struct TCB {
	// contains pointer to top of stack: void* ss_sp 
	uthread_ctx_t *ctx;  
	uthread_t TID;
	int state; 
	int ret; // return value from exit
	int* retContain; // Container in joining thread to receive exit value from joined (dead) thread
	uthread_t collected; // a thread claimed it for collecting
	uthread_t collecting; // in the process of joining with a thread
};

struct TCB *curBlock; // current running block


	/*
	 * return 1 if we found current running process
	 * return 0 otherwise
	 */
int get_running_block(queue_t lib, void* block, void* arg)
{   	
	struct TCB *cur = (struct TCB *) block;
	
	if(cur->state == RUNNING)
	{
		curBlock = (struct TCB*) block;
		return 1;
	}

	return 0;
}

int find_TID(queue_t lib, void* block, void* arg)
{

	struct TCB *cur = (struct TCB *) block;
	uthread_t tid = (intptr_t) arg; // uthread_t didn't compile

	if(cur->TID == tid)
		return 1;

	return 0;

}

/*
 * searches library for Node 
 * in order to retrieve the data (TCB) and 
 * ptr to next queue Node
 */

// changed return type cannot accessed struct Node
int  find_next(queue_t lib, void* tBlock, void*arg)
{

	struct TCB* block = (struct TCB*) tBlock;
	int* flag = (int*) arg;

	if(block->TID == curBlock->TID)
	{
		*flag = 1;
		return 0;
	}
	
	if(*flag == 1)
	{
		if(block->state == READY)
			return 1;
	}

	
	return 0;
}

void uthread_yield(void)
{
	queue_func_t find = &find_next;
	int flag = 0;	
	struct TCB* next; // block we want to yield to

	int iter = queue_iterate(library, find, (void*)&flag, (void**)&next);

	if(iter == -1) // need to look at front of queue
		queue_iterate(library, find, (void*)&flag, (void**)&next);

	uthread_ctx_switch(curBlock->ctx, next->ctx);
	next->state = RUNNING;
	
	if(curBlock->state == RUNNING) // if it's waiting or finished should't happen
		curBlock->state = READY;

	curBlock = next;
	
	if(curBlock->collecting != -1)
		uthread_join(curBlock->collecting, curBlock->retContain);
		
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	//void* arg;
	queue_func_t func = &get_running_block; // function needed to call iterate
  
	int iter = queue_iterate(library, func, NULL, NULL);
	
	if(iter == 0) // double cheking queue_iterate worked
		return curBlock->TID; // curBlock should be running block

	return 0; // since it is a number don't know if it will work
		

	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	int create;
	struct TCB *tBlock = (struct TCB*)malloc(sizeof(struct TCB));
	if(library == NULL) // we are creating our main thread
	{
		struct TCB *mainBlock = (struct TCB*) malloc(sizeof(struct TCB));
		library = queue_create();	
		queue_enqueue(library, mainBlock);
		mainBlock->TID = queue_length(library) - 1;
		mainBlock->collecting = -1;
		mainBlock->collected = -1;
		mainBlock->state = RUNNING;
		mainBlock->ctx = malloc(sizeof(uthread_ctx_t));
		curBlock = mainBlock; // main should be the running thread at this pt
		
	}
	
	void *stack = uthread_ctx_alloc_stack();
	tBlock->ctx = malloc(sizeof(uthread_ctx_t));

	create = uthread_ctx_init(tBlock->ctx, stack, func, arg);
	
	if (create == 0)
	{
		// enqueue new thread
		queue_enqueue(library, tBlock);
		// gets its TID
		tBlock->TID = queue_length(library) - 1;
		tBlock->state = READY;
		tBlock->collecting = -1; // tid of the thread it is collecting
		tBlock->collected = -1; // tid of the thread that wants to collect it
		
		return tBlock->TID;
	} 
	

	return -1;

	/* TODO Phase 2 */
}

void uthread_exit(int retval)
{
		
		if(curBlock->state != RUNNING) 
		{
			uthread_self(); // making sure curBlock is currently running
		}

		curBlock->ret = retval;  
		curBlock->state = FINISHED;
		
		if(curBlock->collected != -1) // a thread want to join it is waiting
		{
			struct TCB* collectingBlock;
			queue_func_t finding = &find_TID;

			queue_iterate(library, finding, (void*)&curBlock->collected, 
				(void**)&collectingBlock); // finds collecting thread
			collectingBlock->state = READY;

		}
	
		uthread_yield();
		
}

int uthread_join(uthread_t tid, int *retval)
{
	queue_func_t func = &find_TID;
	// queue_func_t ready_threads = &any_ready_threads;
	struct TCB * deadTCB;

	queue_iterate(library, func, (void*)&tid ,(void**) &deadTCB);
	
	if( tid == curBlock->TID || tid <= 0 || iter != 0 || deadTCB->collected != -1)
		return -1;
	printf("not getting here\n");
	if(deadTCB->state == FINISHED)
	{
		// deadTCB->ctx->uc_stack.ss_pp is our stack ptr
		if(retval)
			*retval = deadTCB->ret;

		uthread_ctx_destroy_stack(deadTCB->ctx->uc_stack.ss_sp);
		curBlock->collecting = -1;
	}	
	else if(deadTCB->state != FINISHED)
	{
		curBlock->state = WAITING;
		curBlock->retContain = retval;
		uthread_yield();
	}
	return 0;
}

