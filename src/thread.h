#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

#include "slist.h"
#include "strlib.h"
#include "varray.h"

#define thread_yield pthread_yield

#define thread_exit(x) pthread_exit(x)

typedef enum {
	MUTEX_FAST = 0,
	MUTEX_RECURSIVE = 1,
	MUTEX_ERROR_CHECKING = 2
} mutex_type_t;

typedef struct mutex_s mutex_t;
struct mutex_s {
	mutex_type_t	type;
	pthread_mutex_t	mutex;
	pthread_mutexattr_t mutex_attr;
};

typedef struct thread_group_s thread_group_t;
struct thread_group_s {
    varray_t *threads;
    mutex_t *threads_mutex;
    int num_processors;		// number of processors in the system
//  int num_smt;		// number of SMT units per processor
    int max_simul_threads;	// max threads the CPU can run, without running multiple on a single execution unit
				// usually set to the number of processors - 1 (for the main program thread)
				// this does not limit the number of threads, but can be used for deciding how many
				// threads to create when parallel processing
    int running_threads;	// number of currently running threads
    int workers_running;
};

typedef enum {
    THREAD_STOPPED = 0,
    THREAD_RUNNING = 1,
    THREAD_PAUSED  = 2
} thread_state_t;

typedef struct thread_s thread_t;
struct thread_s {
    pthread_t thread;
    pthread_attr_t attrs;
    string_t *buf;	// for read/write style communication
    slist_t *events;	// for event style communication
    mutex_t *events_mutex;
    void *(*func)(void *);
    void *data;
    thread_state_t state;
    thread_group_t *group;
    uint32_t work_requests;
    int cpu;
};

#define mutex_lock(m)	pthread_mutex_lock(&(m)->mutex)
#define mutex_try_lock(m)	pthread_mutex_trylock(&(m)->mutex)
#define mutex_unlock(m)	pthread_mutex_unlock(&(m)->mutex)

void mutex_free(mutex_t *mutex);

#define atomic_add(p,n)	__sync_fetch_and_add(p,n)
#define atomic_sub(p,n)	__sync_fetch_and_sub(p,n)

mutex_t *mutex_new(mutex_type_t type);
void mutex_free(mutex_t *mutex);
thread_t *thread_new(thread_group_t *tg, void *(*func)(void *), void *data);
void thread_free(thread_t *thread);
thread_t *thread_self(void);
void thread_start(thread_t *thread);
int num_processors(void);
void thread_group_destroy(thread_group_t *tg);
thread_group_t *thread_group_new(void);
void thread_group_spawn(thread_group_t *tg, void *(*func)(void *), void *data);
void thread_group_spawn_all(thread_group_t *tg, void *(*func)(void *), void *data);
void thread_group_wait_all(thread_group_t *tg, void (*exit_cb)(void **), void **data);
void thread_workers_wait(thread_group_t *tg);
void *thread_worker_main(void *data);
void thread_group_spawn_workers(thread_group_t *tg);
void thread_group_stop_workers(thread_group_t *tg);
void *thread_main(void *data);
void thread_exit_cb(void **data);

#endif
