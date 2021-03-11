// File:	rpthread_t.h

// List all group member's name:
// username of iLab:
// iLab Server:

#ifndef RTHREAD_T_H
#define RTHREAD_T_H

#define _GNU_SOURCE

/* To use Linux pthread Library in Benchmark, you have to comment the USE_RTHREAD macro */
#define USE_RTHREAD 1

#ifndef TIMESLICE
/* defined timeslice to 5 ms, feel free to change this while testing your code
 * it can be done directly in the Makefile*/
#define TIMESLICE 5
#endif

#define READY 0
#define SCHEDULED 1
#define BLOCKED 2

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

// what i added
#include <ucontext.h>
#include <string.h>
#include<sys/time.h>
#include<signal.h>


typedef uint rpthread_t;

typedef struct threadControlBlock {
	/* add important states in a thread control block */
	// thread Id
	// thread status
	// thread context
	// thread stack
	// thread priority
	// And more ...

	ucontext_t *ctx;
	rpthread_t tid;

	void *value_ptr; //need this for join

	//made it into a global var instead
	//int status; //ready,scheduled,blocked

	// YOUR CODE HERE
} curtcb; 


/* define your data structures here: */
// Feel free to add your own auxiliary data structures (linked list or queue etc...)

// YOUR CODE HERE
typedef struct tidNode{
	tidNode* next; 
	rpthread_t tid; 
}tidNode; 

typedef struct tNode{
	int thread_level; //for priority
	struct tNode* nextNode;
	tidNode* jTids; //list of joined threads 
	curtcb curtcb; 
}tNode; 

typedef struct tQueue{
	struct tNode* head; 
	struct tNode* tail; 
}tQueue; 


/* mutex struct definition */
typedef struct rpthread_mutex_t {
	/* add something here */

	int init;
	//lock for tNode
	int lock; //1=lock, 0=unlocked
	//lock for tQueue
	int lockQueue;

	tQueue *wait;

	// YOUR CODE HERE
} rpthread_mutex_t;


/* Function Declarations: */

int searchQ(rpthread_t tid, tQueue *queue);


/* append onto queue*/
void enqueue(tNode* newNode, tQueue *queue);

/* remove from front of queue */
tNode* dequeue(tQueue *queue);

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr, void
    *(*function)(void*), void * arg);

/* give CPU pocession to other user level threads voluntarily */
int rpthread_yield();

/* terminate a thread */
void rpthread_exit(void *value_ptr);

/* wait for thread termination */
int rpthread_join(rpthread_t thread, void **value_ptr);

/* initial the mutex lock */
int rpthread_mutex_init(rpthread_mutex_t *mutex, const pthread_mutexattr_t
    *mutexattr);

/* aquire the mutex lock */
int rpthread_mutex_lock(rpthread_mutex_t *mutex);

/* release the mutex lock */
int rpthread_mutex_unlock(rpthread_mutex_t *mutex);

/* destroy the mutex */
int rpthread_mutex_destroy(rpthread_mutex_t *mutex);


void sighandler();

void init_schedule();

static void schedule();

void resetTime();

#ifdef USE_RTHREAD
#define pthread_t rpthread_t
#define pthread_mutex_t rpthread_mutex_t
#define pthread_create rpthread_create
#define pthread_exit rpthread_exit
#define pthread_join rpthread_join
#define pthread_mutex_init rpthread_mutex_init
#define pthread_mutex_lock rpthread_mutex_lock
#define pthread_mutex_unlock rpthread_mutex_unlock
#define pthread_mutex_destroy rpthread_mutex_destroy
#endif

#endif
