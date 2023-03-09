#ifndef THREAD_FUNC_H
#define THREAD_FUNC_H

#include "thread.h"

// we are probably going to either need to #ifdef this or change to 128 on 64bit arch
typedef uint64_t thread_func_addr_t;

// i think this works on windows:
//#define THREAD_FUNC_PREFIX _cdecl
#define THREAD_FUNC_PREFIX
#define THREAD_FUNC_POSTFIX __attribute__((cdecl))

#define THREAD_FUNC_ADDR_QUIT 0x1	/* special address to signal worker thread to exit */

#define THREAD_FUNC_RETURN_VOID   0
#define THREAD_FUNC_RETURN_INT32  1
#define THREAD_FUNC_RETURN_INT64  2
#define THREAD_FUNC_RETURN_FLOAT  3
#define THREAD_FUNC_RETURN_DOUBLE 4

#define MAX_ARGS 128 // c99 max?
typedef struct thread_func_s {
    thread_func_addr_t addr;		// address of function: (eg &main)
    int argc;				// argument count
    thread_func_addr_t args[MAX_ARGS];	// arguments
    int8_t arg_lens[MAX_ARGS];		// sizeof(argument)
    thread_func_addr_t retval;		// return value
    int8_t retval_type; 
} thread_func_t;

void thread_func_exec(thread_func_t *func);
void THREAD_FUNC_PREFIX thread_func_exec_asm(thread_func_t *func, int *offsets) THREAD_FUNC_POSTFIX;
thread_t *thread_next(thread_group_t *tg);
thread_func_t *thread_func_new(thread_func_addr_t addr, int argc, ...);
void thread_func_run(thread_t *thread, thread_func_t *func);
void thread_func_free(thread_func_t *func);


// launches function at addr as a new thread
#define thread_run_0arg(group,addr) {\
    uint32_t addr32 = (uint32_t)addr;\
    thread_func_t *func = thread_func_new((thread_func_addr_t)addr32); \
    thread_t *thread = thread_next(group); \
    thread_func_run(thread,func);\
}
#define thread_run_1arg(group,addr,a1) {\
    uint32_t addr32 = (uint32_t)addr;\
    thread_func_t *func = thread_func_new((thread_func_addr_t)addr32,1,sizeof(a1),a1); \
    thread_t *thread = thread_next(group); \
    thread_func_run(thread,func);\
}
#define thread_run_2arg(group,addr,a1,a2) {\
    uint32_t addr32 = (uint32_t)addr;\
    thread_func_t *func = thread_func_new((thread_func_addr_t)addr32,2,sizeof(a1),a1,sizeof(a2),a2); \
    thread_t *thread = thread_next(group); \
    thread_func_run(thread,func);\
}
#define thread_run_3arg(group,addr,a1,a2,a3) {\
    uint32_t addr32 = (uint32_t)addr;\
    thread_func_t *func = thread_func_new((thread_func_addr_t)addr32,3,sizeof(a1),a1,sizeof(a2),a2,sizeof(a3),a3); \
    thread_t *thread = thread_next(group); \
    thread_func_run(thread,func);\
}

/*
 * user-defined thread functions
 *
 * !!! WARNING !!!
 *
 * These run asynchronously, so they should all return void.
 *
 * Make sure you pass arguments of the same size/type that the
 * function wants. The sizeof() operation in the macro is going to
 * a) make compile time type checking not work
 * b) use the size of the value the caller provides, which
 *    may not nessecarily be what the function accepts
 *    so an implicit cast by the compiler will not be performed
 * c) cause the following to fail as well:
 *
 *       function("some string");
 *
 *    use this instead:
 *       char *str = "some string";
 *       function(str); 
 *
 * this is because sizeof("some string") isnt sizeof(char *)
 *
 * hopefully there will be a better way to handle this in the
 * future. If the macros break, build your own thread_func_t
 * and pass it to thread_func_run yourself
 *
 * These macros are to increase code readability, and
 * to reduce code size, by making certain thread operations
 * a one-liner. But they could fail if not properly used.
 *
 */
void THREAD_FUNC_PREFIX thread_hello(char *str) THREAD_FUNC_POSTFIX;
#define thread_hello(a) thread_run_1arg(tg,_thread_hello,a)
#define thread_hello2(a,b) thread_run_2arg(tg,_thread_hello2,a,b)
#define thread_hello3(a,b,c) thread_run_3arg(tg,_thread_hello3,a,b,c)
#define update_particle_velocities(a,b) thread_run_2arg(tg,_update_particle_velocities,a,b)
#define update_particle_positions(a,b) thread_run_2arg(tg,_update_particle_positions,a,b)

#endif
