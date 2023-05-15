#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mem.h"
#include "error.h"

void
error_set(err_t **dst, char *fmt, ...)
{
    if (dst == NULL)
	return;

    /* Guess we need no more than 100 bytes. */
    int n, size = 100;
    char *p;
    va_list ap; 

    if ((p = malloc (size)) == NULL)
	return;

    for (;;) {
	/* Try to print in the allocated space. */
	va_start(ap, fmt);
	n = vsnprintf (p, size, fmt, ap);
	va_end(ap); 

	/* If that worked, return the string. */
	if (n > -1 && n < size)
	    goto success;

	/* Else try again with more space. */
	if (n > -1)             // glibc 2.1
	    size = n+1;         // precisely what is needed
	else                    // glibc 2.0
	    size *= 2;          // twice the old size

	if ((p = realloc (p, size)) == NULL)
	    return;
    }   

    return;

success:
    {
	err_t *err = mem_new0(err_t);
	err->msg = p;
	*dst = err;
    }
}

void
error_free(err_t *err)
{
    if (err == NULL)
	return;

    if (err->msg != NULL)
	mem_free(err->msg);

    mem_free(err);
}

void
error_propagate(err_t **dst, err_t *err)
{
    if (dst == NULL)
	return;

    *dst = err;
}

#ifdef WIN32
char *
error_win32_strerror(void)
{
    DWORD code = GetLastError();

    /*
       DWORD WINAPI FormatMessage(
       _In_      DWORD dwFlags,
       _In_opt_  LPCVOID lpSource,
       _In_      DWORD dwMessageId,
       _In_      DWORD dwLanguageId,
       _Out_     LPTSTR lpBuffer,
       _In_      DWORD nSize,
       _In_opt_  va_list *Arguments
       );
     */
    FormatMessage(
	    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
	    NULL,
	    code,
	    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	    &msg,
	    0,
	    NULL);

    return msg;
}
#endif

#ifdef ERROR_MAIN
int
main(int argc, char **argv)
{
    return 0;
}
#endif
