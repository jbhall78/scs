#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <glib.h>

#include <windows.h>

static FILE *log;
static char base[BUFSIZ];

void
sys_init(void)
{
    char *p;

    /* grab the path to the executable */
    if (! GetModuleFileName(NULL, base, BUFSIZ-1)) {
	MessageBox(NULL, "Can't find module path", "ERROR",
		MB_OK|MB_ICONEXCLAMATION);
	exit(1);
    }

    if ((p = strrchr(base, '\\')))
	*p = '\0';

    strcat(base, "\\..\\");

    chdir(base);

    /* open the system log file */
    if (! (log = fopen("scs.log", "w"))) {
	MessageBox(NULL, "Can't open log file", "ERROR",
		MB_OK|MB_ICONEXCLAMATION);
	exit(1);
    }

}

void
sys_print(int level, char *str)
{
    fputs(str, log);
}

void
sys_shutdown(void)
{
    fclose(log);
}

char *
sys_datadir(void)
{
    char buf[BUFSIZ];

    snprintf(buf, sizeof(buf), "%s\\", base);

    return strdup(buf);
}
