#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include <pthread.h>

#include "mem.h"
#include "thread.h"
#include "thread_func.h"

#include "registers-i386.h"

registers_t regs;

/*
THREAD_FUNC // implies void
_some_function(char *foo, char *bar)
{

}
// if you want to make a natural looking function call executed as a thread
#define some_function(a,b) thread_group_run_2arg(global_thread_group,_some_function,a,b)
// or
#define some_function(group,a,b) thread_group_run_2arg(group,_some_function,a,b)
some_function(a,b)
*/

/*
 * Append the thread function call to the worker threads event queue
 */
void
thread_func_run(thread_t *thread, thread_func_t *func)
{
    slist_t *item = slist_new();
    item->data = func;

//    printf("thread_func_run\n");

    mutex_lock(thread->events_mutex);
//    printf("thread_func_run[0x%016lx] appending event\n", thread->thread);
    thread->events = slist_append(thread->events, item);
//    printf("n_events: %d\n", slist_len(thread->events));
    mutex_unlock(thread->events_mutex);
}

/*
 * Prepare for & call the assembly language routine 
 */
void
thread_func_exec(thread_func_t *func)
{
    // build offsets array, which contains the
    // offsets of each of the structure elements
    // don't reorder these, the asm routines
    // depend on the order
    int offsets[6];

    // could also fill with the offsets directly
    // if nessecary for portability
    offsets[0] = offsetof(thread_func_t, addr);
    offsets[1] = offsetof(thread_func_t, argc);
    offsets[2] = offsetof(thread_func_t, args);
    offsets[3] = offsetof(thread_func_t, arg_lens);
    offsets[4] = offsetof(thread_func_t, retval);
    offsets[5] = offsetof(thread_func_t, retval_type);

#if 0
    for (i = 0; i < 5; i++) {
	printf("offset[%d] = '%d'\n", i, offsets[i]);
    }
#endif

/* don't do this here for efficency
    // function arguments are pushed onto the stack reversed, 
    // but the asm does it in order, so we reverse the order here
    int i, j;
    int8_t tmp_arg_len;
    thread_func_addr_t tmp_addr;

    for (i = func->argc-1, j = 0; j < func->argc/2; i--, j++) {
        tmp_addr = func->args[i];
	tmp_arg_len = func->arg_lens[i];
        func->args[i] = func->args[j];
	func->arg_lens[i] = func->arg_lens[j];
        func->args[j] = tmp_addr;
        func->arg_lens[j] = tmp_arg_len;
    }
*/

/*
    printf("=====  ASM CALL =================================\n");
    registers_get(&regs);
    registers_print(&regs);
*/
    thread_func_exec_asm(func, offsets);
/*
    registers_get(&regs);
    registers_print(&regs);
    printf("=================================================\n");
*/
}

// returns first available thread in group
thread_t *
thread_next(thread_group_t *tg)
{
    thread_t *thread;
    static int i = 0;

    i++;
    if (i >= tg->threads->len)
	i = 0;

    thread = tg->threads->data[i];
    return thread;

#if 0
    while (1) {
	for (; i < tg->threads->len; i++) {
	    thread = tg->threads->data[i];
	    if (thread->state == THREAD_STOPPED) {
//		printf("thread_next[%d]: 0x%016lx\n", i, thread->thread);
		return thread;
	    }
	}
	i = 0;
//	thread_yield();
    }
#endif

    return NULL; /* unreached */
}

void
thread_func_free(thread_func_t *func)
{
    mem_free(func);
}

#if 0
thread_func_t *
thread_func_new(thread_func_addr_t addr, int argc, ...)
{
    thread_func_t *tf;
    va_list ap;
    int i;

    tf = mem_new0(thread_func_t);

    tf->addr = addr;
    tf->argc = argc;
    tf->retval = 0;
    tf->retval_type = THREAD_FUNC_RETURN_VOID;

    // fill in arguments & argument lengths
    va_start(ap, argc);	// name of last argument before start of variable length argument list
    for (i = 0; i < argc; i++) {
	int len = va_arg(ap, int);
	tf->arg_lens[i] = len;
	switch (len) {
/*
	    case 1:
		tf->args[i] = va_arg(ap, int8_t);
		break;
	    case 2:
		tf->args[i] = va_arg(ap, int16_t);
		break;
*/
	    case 4:
		tf->args[i] = va_arg(ap, int32_t);
//		printf("arg4: %d\n", tf->args[argc-i-1]);
		break;
	    case 8:
		tf->args[i] = va_arg(ap, int64_t);
		break;
	    default:
		fprintf(stderr, "ERROR: thread_func_init was passed an invalid argument length of %d\n", len);
		fprintf(stderr, "       this is most likely a programming error, please read thread_func.h\n");
		fprintf(stderr, "       for more details\n");
		exit(1);
	}
    }
    va_end(ap);

    return tf;
}
#endif

#if 1
thread_func_t *
thread_func_new(thread_func_addr_t addr, int argc, ...)
{
    thread_func_t *tf;
    va_list ap;
    int i;

    tf = mem_new0(thread_func_t);

    tf->addr = addr;
    tf->argc = argc;
    tf->retval = 0;
    tf->retval_type = THREAD_FUNC_RETURN_VOID;

    // fill in arguments & argument lengths
    va_start(ap, argc);	// name of last argument before start of variable length argument list
    for (i = 0; i < argc; i++) {
	int len = va_arg(ap, int);
	// function arguments are pushed onto the stack in reverse order
	// the asm does it in order, so we reverse here
	tf->arg_lens[argc-i-1] = len;
	switch (len) {
/*
	    case 1:
		tf->args[i] = va_arg(ap, int8_t);
		break;
	    case 2:
		tf->args[i] = va_arg(ap, int16_t);
		break;
*/
	    case 4:
		tf->args[argc-i-1] = va_arg(ap, int32_t);
//		printf("arg4: %d\n", tf->args[argc-i-1]);
		break;
	    case 8:
		tf->args[argc-i-1] = va_arg(ap, int64_t);
		break;
	    default:
		fprintf(stderr, "ERROR: thread_func_init was passed an invalid argument length of %d\n", len);
		fprintf(stderr, "       this is most likely a programming error, please read thread_func.h\n");
		fprintf(stderr, "       for more details\n");
		exit(1);
	}
    }
    va_end(ap);

    return tf;
}
#endif
