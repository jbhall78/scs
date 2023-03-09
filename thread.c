#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

#include "mem.h"
#include "strlib.h"
#include "varray.h"
#include "slist.h"
#include "hash.h"
#include "clock.h"
#include "timer.h"

#include "thread.h"
#include "thread_func.h"

static varray_t *thread_groups;
static hash_t *running_threads;
static mutex_t *running_threads_mutex;

#ifdef THREAD_MAIN
static int32_t  int32 = 0;
static mutex_t *int32_mutex;
#endif

/*
inline void
mutex_lock(mutex_t *mutex)
{
    pthread_mutex_lock(&mutex->mutex);
}

inline void
mutex_unlock(mutex_t *mutex)
{
    pthread_mutex_unlock(&mutex->mutex);
}
*/

// this mutex api is perhaps horribly inefficient :(
mutex_t *
mutex_new(mutex_type_t type)
{
    mutex_t *mutex;
    mutex = mem_new(mutex_t);
    mutex->type = type;

    pthread_mutexattr_init(&mutex->mutex_attr);
    switch (type) {
	case MUTEX_FAST:
	    pthread_mutexattr_settype(&mutex->mutex_attr, PTHREAD_MUTEX_FAST_NP);
	    break;
	case MUTEX_RECURSIVE:
	    pthread_mutexattr_settype(&mutex->mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
	    break;
	case MUTEX_ERROR_CHECKING:
	    pthread_mutexattr_settype(&mutex->mutex_attr, PTHREAD_MUTEX_ERRORCHECK_NP);
	    break;
    }
    pthread_mutex_init(&mutex->mutex, &mutex->mutex_attr);

    return mutex;
}

void
mutex_free(mutex_t *mutex)
{
    mem_free(mutex);
}

void
thread_free_cb(void *data)
{
    thread_t *thread = data;

    mutex_free(thread->events_mutex);

    // XXX
}

thread_t *
thread_new(thread_group_t *tg, void *(*func)(void *), void *data)
{
    int retval;

    thread_t *thread = mem_new0(thread_t);
    mutex_lock(tg->threads_mutex);
    varray_append(tg->threads, thread);
    mutex_unlock(tg->threads_mutex);
    thread->func = func;
    thread->data = data;
    thread->group = tg;
    thread->events_mutex = mutex_new(MUTEX_FAST);

    retval = pthread_attr_init(&thread->attrs);
//    printf("pthread_attr_init: %d\n", retval);

    return thread;
}

void
thread_free(thread_t *thread)
{
    slist_destroy(thread->events, NULL);
    mutex_free(thread->events_mutex);
    mem_free(thread);
}

thread_t *
thread_self(void)
{
    thread_t *thread = NULL;
    pthread_t id;

    id = pthread_self();

    mutex_lock(running_threads_mutex);

//    printf("get id: 0x%016lx\n", id);
    hash_get(running_threads, &id, sizeof(pthread_t), &thread, NULL);

    mutex_unlock(running_threads_mutex);

    assert(thread != NULL);
    return thread;
}

void
thread_start(thread_t *thread)
{
    int retval;

    assert(thread->thread == 0);

    if (running_threads == NULL) {
	running_threads = hash_new(NULL, NULL);
	running_threads_mutex = mutex_new(MUTEX_FAST);
    }
    mutex_lock(running_threads_mutex);

    retval = pthread_create(&thread->thread,&thread->attrs,thread->func,thread->data);
    //	    printf("pthread_create: %d\n", retval);
    thread->group->running_threads++;

    //	    printf("set id: 0x%016lx\n", thread->thread);
    hash_set(running_threads, &thread->thread, sizeof(thread->thread), thread, sizeof(thread_t *));

    mutex_unlock(running_threads_mutex);
}

int
num_processors(void)
{
    FILE *fh;
    char *filename = "/proc/cpuinfo";
    char buf[BUFSIZ];
    int count = 0;

    /*
     * use /proc/cpuinfo for compatibility
     * maybe later get fancier and look into
     * using /sys/devices/system/cpu/online
     */
    if (! (fh = fopen(filename, "r"))) {
	fprintf(stderr, "ERROR: cannot open %s\n", filename);
	exit(1);	
    }

    while (fgets(buf, sizeof(buf), fh) != NULL) {
	int res;
	res = strncmp(buf, "processor\t", strlen("processor\t"));
	if (res == 0) {
#if 0
	    char *p = strchr(buf, '\n');
	    *p = 0;
	    p = strchr(buf, ':');
	    p += 2;
	    printf("found processor id: %s\n", p);
#endif
	    count++;
	}
    }

    fclose(fh);

    return count;
//    return 1;
}

thread_group_t *
thread_group_new(void)
{
    int n;
    thread_group_t *tg = mem_new0(thread_group_t);
    n = num_processors();
    tg->num_processors = n;
    tg->max_simul_threads = (n > 1) ? n - 1 : 1;
//    tg->max_simul_threads = n * 2;
    tg->threads = varray_new(NULL, 0, NULL);
    tg->threads_mutex = mutex_new(MUTEX_FAST);

    if (thread_groups == NULL) {
	thread_groups = varray_new(NULL, 0, NULL);
    }
    varray_append(thread_groups, tg);

    return tg;
}

void
thread_group_free(thread_group_t *tg)
{
    mutex_free(tg->threads_mutex);
    varray_free(tg->threads);	// XXX - they might not be shut down
    varray_delete(thread_groups, tg);
    mem_free(tg);
}

void
thread_group_spawn(thread_group_t *tg, void *(*func)(void *), void *data)
{

    thread_t *thread = thread_new(tg, func, data);
    thread_start(thread);
}

void
thread_group_spawn_all(thread_group_t *tg, void *(*func)(void *), void *data)
{
    cpu_set_t set;
    int i, n;

    for (i = 0; i < tg->max_simul_threads; i++) {
#if 1
	CPU_ZERO(&set);
	n = i + 1;
	if (n >= tg->num_processors)
	    n -= tg->num_processors;
	CPU_SET_S(n, tg->num_processors, &set);
#endif

	thread_t *thread = thread_new(tg, func, data);
	thread_start(thread);

#if 1
	if (pthread_setaffinity_np(thread->thread, tg->num_processors, &set) != 0) {
	    fprintf(stderr, "ERROR: could not set CPU(%d) affinity for thread[0x%08x]: %s\n", n, (int32_t)thread->thread, strerror(errno));
	    exit(1);
	}
	thread->cpu = n;
#endif
    }
}

void
thread_group_wait_all(thread_group_t *tg, void (*exit_cb)(void **), void **data)
{
    int i, retval;

     while (tg->running_threads > 0) {
         for (i = 0; i < tg->threads->len; i++) {
	     thread_t *thread = tg->threads->data[i];

	     if (thread->thread == 0)
		 continue;
 
             retval = pthread_tryjoin_np(thread->thread, data);
//   	     printf("pthread_tryjoin_np: %d\n", retval);
 
             if (retval == 0) {
		 int j;
		 // thread exited
                 tg->running_threads--;
		 thread->state = THREAD_STOPPED;
                 printf(">>> thread id: %016lx terminated (%d left)\n", thread->thread, tg->running_threads);
                 thread->thread = 0;
		 mutex_lock(running_threads_mutex);
		 hash_delete(running_threads, &thread->thread, sizeof(thread->thread));
		 mutex_unlock(running_threads_mutex);
		 mutex_lock(tg->threads_mutex);
		 for (j = 0; j < tg->threads->len; j++) {
		     if (tg->threads->data[j] == thread) {
			 varray_delete_at(tg->threads, 1, j);
		     }
		 }
		 mutex_unlock(tg->threads_mutex);
             }
         }
	 pthread_yield();	// relinquish cpu so threads can finish faster
     }
}

// wait for workers to complete
void
thread_workers_wait(thread_group_t *tg)
{
    while (tg->workers_running > 0) {
//	printf("%d workers\n", tg->workers_running);
//	sleep(1);
	;
    }
}

void
thread_group_stop_workers(thread_group_t *tg)
{
    int i;

    // craft a special shutdown message and send it to all threads
    for (i = 0; i < tg->threads->len; i++) {
	thread_t *thread = tg->threads->data[i];

	if (thread->thread == 0)
	    continue;

      	thread_func_t *func = thread_func_new(THREAD_FUNC_ADDR_QUIT, 0);
	slist_t *item = slist_new();
	item->data = func;

	thread_func_run(thread, func);
    }

    // wait for them all to finish (could hang forever)
    thread_group_wait_all(tg, thread_exit_cb, NULL);
}

void *
thread_worker_main(void *data)
{
    thread_t *thread = thread_self();

    atomic_add(&thread->group->workers_running, 1);
    thread->state = THREAD_RUNNING;

    // TODO: implement some sort of sleeping / delay in here
    //       if we have been idle for a while. i attempted
    //       putting one in, but i'm tired and don't feel like
    //       thinking about it right now.
    //
    //        also would probably be worth calculating
    //        idle/sleep/running times
    //
    //        and and! pin the worker threads to a cpu
    //        seems like the linux scheduler might be doing
    //        something wierd, in addition to my code not 
    //        working.

    while (1) {
	mutex_lock(thread->events_mutex);
	if (thread->events != NULL) {
	    // grab the next worker function in queue
	    thread_func_t *func = thread->events->data;
	    thread->events = slist_remove(thread->events, thread->events, NULL);
	    mutex_unlock(thread->events_mutex);

	    if (func->addr == THREAD_FUNC_ADDR_QUIT) {
		// make sure we are in correct state when we shut down
		// may be redundant
		if (thread->state != THREAD_STOPPED) {
		    thread->state = THREAD_STOPPED;
		    atomic_sub(&thread->group->workers_running, 1);
		}
		return NULL;
	    }

	    if (thread->state == THREAD_STOPPED) {
		thread->state = THREAD_RUNNING;
		atomic_add(&thread->group->workers_running, 1);
	    }

	    // run function
//	    printf("executing function\n");
	    thread_func_exec(func);
	    thread->work_requests++;


	    // cleanup
	    thread_func_free(func);
	} else {
	    if (thread->state == THREAD_RUNNING) {
		thread->state = THREAD_STOPPED;
		atomic_sub(&thread->group->workers_running, 1);
	    }
	    mutex_unlock(thread->events_mutex);
	}
//	thread_yield();
    }
}

void
thread_group_spawn_workers(thread_group_t *tg)
{
     thread_group_spawn_all(tg, thread_worker_main, NULL);  // create a new thread on each execution unit
}

void *
thread_main(void *data)
{
    uint32_t i;
    thread_t *thread = thread_self();

    printf("hello from thread: 0x%016lx\n", thread->thread);

    for (i = 0; i < USHRT_MAX; i++)
	;

    thread_exit(NULL);
}

void
thread_exit_cb(void **data)
{
    printf("thread exited\n");
}

#ifdef THREAD_MAIN
void
_thread_hello3(int a, int b, int c)
{
    thread_t *thread = thread_self();
    printf("thread_hello3[0x%016lx]: %d    %d    %d\n", thread->thread, a, b, c);

}
void
_thread_hello2(int a, int b)
{
    thread_t *thread = thread_self();
    printf("thread_hello2[0x%016lx]: %d    %d\n", thread->thread, a, b);

}
void
_thread_hello(char *str)
{
    int i;
    thread_t *thread = thread_self();
    int a, b;
//    printf("thread_hello[0x%016lx]: [0x%08x] %s\n", thread->thread, str, str);


    for (i = 0; i < 4000; i++) {
#ifdef SLOW
    	mutex_lock(int32_mutex);
	int32++;
    	mutex_unlock(int32_mutex);
#else
	atomic_add(&int32, 1);
#endif
	a = i;
	b += i;
	b -= a;
	asm("nop;nop;nop;nop;nop");
    }
//    printf("called: %d\n", int32);
}

#define MAXIMUM 4000
int
main(int argc, char **argv)
{
    thread_group_t *tg;
    char *str = "hello world!";
    int max = 60000;
    int i;
    clock_driver_t *clock = &clock_gettime_driver;
    struct timespec start, now, ts;
    uint64_t array[MAXIMUM], n;

    mem_init();

/*
    clock->open();
    clock_gettime_read(&start);
    for (i = 0; i < MAXIMUM; i++) {
	clock_gettime_read(&start);
	clock_gettime_read(&ts);
	clock_gettime_read(&now);
	n = s_to_ns(now.tv_sec - start.tv_sec);
	n += (now.tv_nsec - start.tv_nsec);

	array[i] = n;
	//printf("clock_gettime_read %lldns\n", n);
    }

    // need a better method for calculating averages
    // a spike or two in the data throws the average way off
    n = 0;
    for (i = 0; i < MAXIMUM; i++) {
	n += array[i];
    }
    n /= MAXIMUM;
    printf("clock_gettime_read %lldns\n", n);
*/


    tg = thread_group_new();			// create new thread group
/*
    thread_group_spawn_all(tg, thread_main, NULL);	// create a new thread on each execution unit
    thread_group_wait_all(tg, thread_exit_cb, NULL);	// wait for all threads to complete execution
*/

    thread_group_spawn_workers(tg);

    int a = 1000;
    int b = 2000;
    int c = 3000;
    thread_hello2(a, b);
    sleep(1);
    thread_workers_wait(tg);

    thread_hello3(a, b, c);
    sleep(1);
    thread_workers_wait(tg);



    // load a bunch of threads
    for (i = 0; i < 12; i++) {
	thread_hello(str);
    }
    sleep(1);
    thread_workers_wait(tg);

//    return 0;


//    return 0;


    int32_mutex = mutex_new(MUTEX_FAST);
    for (i = 0; i < MAXIMUM; i++) {
	clock_gettime_read(&start);
	thread_hello(str);
	clock_gettime_read(&now);
	n = s_to_ns(now.tv_sec - start.tv_sec);
	n += (now.tv_nsec - start.tv_nsec);

	array[i] = n;
//	printf("thread_hello latency %lldns\n", n);
    }
    n = 0;
    for (i = 0; i < MAXIMUM; i++) {
	n += array[i];
    }
    n /= MAXIMUM;
    printf("thread_hello latency %lldns\n", n);
    int32_t last = 0;
/*    while (int32 < (MAXIMUM * 4000)) {
	printf("int32: %d [delta %d@500ms]\n", int32, int32 - last);
	last = int32;
	sys_timer_sleep_ms(500);
    } 
    printf("int32: %d\n", int32);*/
    for (i = 0; i < tg->threads->len; i++) {
	thread_t *thread = tg->threads->data[i];
	printf("thread[0x%08x]->requests = %d\n", thread->thread, thread->work_requests);
    }
#if 0
    while (1) {
	printf("doing work\n");
	for (i = 0; i < max; i++) {
	    //printf("trying: 0x%08x\n", str);
	}
	printf("sleeping\n");
	for (i = 0; i < 6; i++) {
	    sleep(1);
	}
	for (i = 0; i < tg->threads->len; i++) {
	    thread_t *thread = tg->threads->data[i];
	    printf("thread[0x%08x]->requests = %d\n", thread->thread, thread->work_requests);
	}
    }
#endif

    thread_group_stop_workers(tg);

    thread_group_free(tg);

    return 0;
}
#endif
