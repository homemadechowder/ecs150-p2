#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100000 // in microseconds 

void preempt_disable(void)
{
	
}

void preempt_enable(void)
{
	
}

void preempt_start(void)
{
	struct sigaction sa;
 	struct itimerval timer;

	memset(&sa, 0, sizeof (sa));
 	sa.sa_handler = &timer_handler;
	sigaction (SIGVTALRM, &sa, NULL);

 	timer.it_value.tv_usec = HZ; // Counts down from 100ms
	timer.it_interval.tv_usec = HZ; //Repeats every 100ms after first expiration
	//Fire alarms of type SIGVTALRM 100/s	
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
	
	//Receive alarms of SIGVTALRM type 
	timer_handler(SIGVTALRM)
	{
		uthread_yield();
	}
	
}

