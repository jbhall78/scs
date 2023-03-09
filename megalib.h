#ifndef MEGALIB_H
#define MEGALIB_H

/* standard C includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

#include "mem.h"	/* memory allocation / deallocation */

#include "registers-i386.h"	/* x86 register access */

#include "mathlib.h"	/* mathmatics (vectors, matrices, quaternions) */

#include "strlib.h"	/* dynamically allocated strings */
#include "array.h"	/* dynamically allocated arrays */
#include "varray.h"	/* dynamically allocated pointer arrays */
#include "dbuf.h"	/* dynamically allocated binary data buffer */

#include "slist.h"	/* linked list */
#include "dlist.h"	/* doubly linked list (next & prev) */
#include "hash.h"	/* tree indexed associative array */

#include "filelib.h"	/* file access */
#include "dirlib.h"	/* directory access */
#include "fs_hash.h"	/* hash which uses filesystem for storage */

#include "clock.h"	/* system clock */
#include "timer.h"	/* timer API which uses clock */

#include "thread.h"	/* thread API */
#include "thread_func.h"	/* macros for calling functions in a thread */

#include "parser.h"	/* text token based parser */
#include "url.h"	/* url parser */

#include "tcp_unix.h"	/* tcp api */
#include "http.h"	/* http */

#include "net.h"

#include "var.h"
#include "var_auto.c"

#include "wav.h"	/* WAV audio file loader */

#define print	printf


#endif
