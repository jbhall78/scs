#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0
int
my_memcmp(char *p1, char *p2, int len)
{
    char buf[BUFSIZ];

    memset(buf, 0, sizeof(buf));
    memcpy(buf, p1, len);
    printf("buf: %d %s\n", len, buf);
    return memcmp(p1, p2, len);
}
#endif

char **
split(char *str, char *delim, int max, int *items)
{
    int str_len, delim_len;
    int remaining;
    int n;
    char **results;
    int size;
    char *p1, *p2;

    str_len   = strlen(str);
    delim_len = strlen(delim);

    // count occurances (the following loop is documented)
    p1 = &str[0];
    p2 = p1;
    n = 0;
    while (1) {
	remaining = str_len - (p2 - &str[0]);
	if (remaining < delim_len || n+1 == max) {
	    while (*p2 != '\0')
		p2++;
	}

	size = p2 - p1;

	if (*p2 == '\0') {
	    n++;
	    break;
	}

	if (memcmp(p2, delim, delim_len) == 0) {
	    n++;
	    p2 += delim_len;
	    p1 = p2;
	    continue;
	}

	p2++;
    }
    *items = n;

    size = sizeof(char *) * (n + 1);	// NULL terminate results
    results = malloc(size);
    memset(results, 0, size);

    // fill in results
    p1 = &str[0];	// start of copy buffer
    p2 = p1;		// end of copy buffer
    n = 0;		// current result index
    while (1) {
	remaining = str_len - (p2 - &str[0]);	// remaining characters in string

	// check to see if we should continue
	if (remaining < delim_len || n+1 == max) {
	    // automatically fast forward to end
	    while (*p2 != '\0')
		p2 += 1;
	}
	size = p2 - p1;	// size of copy buffer

	// handle remainder if nessecary
	if (*p2 == '\0') {
	    // if copy buffer has length > 0 create a new result
	    if (size > 0) {
		results[n] = malloc(size + 1);
		memset(results[n], 0, size + 1);
		memcpy(results[n], p1, size);
	    }
	    n++;
	    break;
	}

	// check for delimiter at end of copy buffer
	if (memcmp(p2, delim, delim_len) == 0) {
	    // if copy buffer has length > 0 create a new result
	    if (size > 0) {
		results[n] = malloc(size + 1);
		memset(results[n], 0, size + 1);
		memcpy(results[n], p1, size);
	    }
	    n++;
	    p2 += delim_len;	// skip over delimiter
	    p1 = p2;		// set new beginning of copy buffer
	    continue;
	}

	p2++;	// make copy buffer larger
    }

    return results;
}

#ifdef SPLIT_MAIN
int
main(int argc, char **argv)
{
    int i;
    int items;
    char **results;

    if (argc < 3) {
	fprintf(stderr, "usage: split <string> <delim> <max>\n");
	exit(1);
    }
    int max = 0;
    char *str = argv[1];
    char *delim = argv[2];
    if (argc > 3)
	max = atoi(argv[3]);
    printf("[%s] [%s] [%d]\n", str, delim, max);

    results = split(str, delim, max, &items);
    printf("items: %d\n", items);

    for (i = 0; i < items; i++) {
	printf("item[%d] = [%s]\n", i, results[i]);
    }

    if (results != NULL)
	free(results);

    return 0;
}
#endif
