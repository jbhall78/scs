#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strlib.h"

void
strip(char *str, int position, int flags)
{
    int len;
    int i, offset;

    if (str == NULL)
	return;

    len = strlen(str);

    if (position == STRIP_START) {
	// strip things from beginning of string
	offset = 0;
	for (i = 0; i < len; i++) {
	    if (flags & STRIP_NEWLINE && (str[i] == '\r' || str[i] == '\n')) {
		offset++;
		continue;
	    }
	    if (flags & STRIP_WHITESPACE && (str[i] == ' ' || str[i] == '\t')) {
		offset++;
		continue;
	    }
	    break;
	}

	// we could just return a pointer but this way is compatible with malloc
	if (offset > 0) {
	    // move string backwards
	    for (i = 0; i < len - offset; i++)
		str[i] = str[offset+i];

	    // fill remainder of string with nulls
	    for (; i < len; i++) 
		str[i] = '\0';
	}
    } else if (position == STRIP_END) {
	// strip things from end of string
	for (i = len-1; i > 0; i--) {
	    if (flags & STRIP_NEWLINE && (str[i] == '\r' || str[i] == '\n')) {
		str[i] = '\0';
		continue;
	    }
	    if (flags & STRIP_WHITESPACE && (str[i] == ' ' || str[i] == '\t')) {
		str[i] = '\0';
		continue;
	    }
	    break;
	}	
    }
}
