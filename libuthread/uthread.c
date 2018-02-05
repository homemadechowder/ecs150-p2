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

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	while (1)
	{
	
	}
	/* TODO Phase 2 
	Execute an infinite loop in which:
	If there are no more threads which are ready to run in the
	system, break the loop and return
	Otherwise simply yield to next available thread
	*/
	/* TODO Phase 3 */
}

