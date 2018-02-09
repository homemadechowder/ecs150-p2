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
#define WAITINIG 4


queue_t library; // it is a pointer to the queue 

struct TCB {
	// contains pointer to top of stack: void* ss_sp 
	uthread_ctx_t *ctx;  
	uthread_t TID;
	int state; 
	int ret;
	bool joined; // to make sure if it was collected or not
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
int  find_next(queue_t lib, voi)
{
//	struct Node* cur = lib->front;

/*	while(cur)
	{
		if(cur->data->TID == tid)
			return 1;

		cur = cur->next;
	}*/
	
	return 0;
}

/*
 * 1 if ready threads left
 * 0 otherwise
 */

int any_ready_threads(queue_t lib, void* next, void* arg)
{
	struct TCB* cur = (struct TCB*) next;
 	
		if(cur->->state == READY )
			return 1;
	
	return 0;
}




void uthread_yield(void)
{
	//struct Node* running, *temp; 

	if(curBlock->state != RUNNING) 
		uthread_self(); // if curBlock not running make point to running
	
	//running = find_node(library, curBlock->TID); // Node of running TCB
	
	/*while(1)
	{
		temp = running->next;
		if(temp) 
		{
			if(temp->data->state == READY)
			{
				uthread_ctx_switch(curBlock->ctx, temp->data->ctx);
				temp->data->state = RUNNING;
				curBlock->state = READY;
				curBlock = temp->data;
				break;

			}
			
			temp = temp->next;
		}
		else // reached the end of the queue begin at front
			temp = library->front;
		
	}*/

	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	void* arg;
	queue_func_t func = &get_running_block; // function needed to call iterate
  
	int iter = queue_iterate(library, func, arg, NULL);
	
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
		// this is questionable ??????????
		queue_enqueue(library, tBlock);
		return 0; // TID of main is 0
	}
	
	void *stack = uthread_ctx_alloc_stack();
	create = uthread_ctx_init(tBlock->ctx, stack, func, arg);
	
	if (create == 0)
	{
		// enqueue new thread
		queue_enqueue(library, tBlock);
		// gets its TID
		tBlock->TID = library->length - 1;
		tBlock->state = READY;
		tBlock->joined = false; // flag for retrieved threads
		// return TID 
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
}

int uthread_join(uthread_t tid, int *retval)
{
	queue_t func = &find_TID;
	queue_t ready_threads = &any_ready_threads();
	struct TCB * deadTCB;
	void* arg;	

	int iter = queue_iterate(library, func, (void*)&tid ,(void**) &deadTCB);
	
	if( tid == uthread_self() || tid <= 0 || !uthread_self()
		||deadTCB->joined || iter != 0)
		return -1;
	
	// once the thread is joined raise flag to 

	while (1)
	{
		
		int find_threads = queue_iterate(library, ready_threads, arg, NULL); 
		if (find_threads == -1) // iteration failed
			break;
	
		uthread_yield();
		
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

