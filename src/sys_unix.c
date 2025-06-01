#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "scs.h"
#include "clock.h"
#include "shared.h"

#include "sys_gl.h"

void
sys_init(void)
{
    scs.clock = &clock_gettime_driver;
    scs.clock->open();

/*    gl = &_gl;
    gl->init(); */
}

void
sys_print(int level, char *str)
{
    if (level == 0)
	fputs(str, stdout);
    else
	fputs(str, stderr);
}

int
sys_mkdir(const char *path) {
    // Use the mkdir function from sys/stat.h
    if (mkdir(path, 0755) == 0) {
        // Directory created successfully
        return OK;
    } else {
        // Error occurred
        return FAIL;
    }
}

int
sys_copy_file(const char *source_path, const char *dest_path) {
    int source_fd = -1;
    int dest_fd = -1;
    ssize_t bytes_read;
    ssize_t bytes_written;
    char buffer[BUFSIZ];

    // Open the source file for reading
    source_fd = open(source_path, O_RDONLY);
    if (source_fd == -1) {
        perror("Error opening source file");
        goto error;
    }

    // Open the destination file for writing (create if it doesn't exist, truncate if it does)
    dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening destination file");
        goto error;
    }

    // Read from source and write to destination
    while ((bytes_read = read(source_fd, buffer, BUFSIZ)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Error writing to destination file");
            goto error;
        }
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Write error: wrote less bytes than read\n");
            goto error;
        }
    }

    if (bytes_read == -1) {
        perror("Error reading from source file");
        goto error;
    }

    // Close file descriptors
    if (close(source_fd) == -1) {
        perror("Error closing source file");
    }
    if (close(dest_fd) == -1) {
        perror("Error closing destination file");
    }

    return OK; // Success

error:
    if (source_fd != -1) {
        close(source_fd);
    }
    if (dest_fd != -1) {
        close(dest_fd);
    }
    return FAIL; // Failure
}


char *
sys_datadir(void)
{
    char buf[BUFSIZ];
    char *cfgdir = getenv("XDG_CONFIG_HOME");
    char *home = getenv("HOME");
    char *p;

    if (! cfgdir && !home)
	return NULL;

    if (cfgdir)
	    p = cfgdir;
    else
	    p = home;

    snprintf(buf, sizeof(buf), "%s/.scs/", p);

    return strdup(buf);
}

void
sys_shutdown(void)
{

}

GList *
_sys_scandir(GList *list, char *path, char *relpath, gboolean recursive, GError **err)
{
    GError *tmp = NULL;
    GDir *dir;
    old_file_t *file;
    const char *name;
    char *newpath;
    char *newrelpath;

    if (! (dir = g_dir_open(path, 0, &tmp))) {
	g_propagate_error(err, tmp);
	return NULL;
    }

    while ((name = g_dir_read_name(dir)) != NULL) {
	// GStatBuf st;  apparently this isn't in this version of glib
	// g_stat  ..  this either
	struct stat st;
	int ret;

	if (strcmp(name, ".") == 0 ||
		strcmp(name, "..") == 0)
	    continue;

	// determine new path
	newpath = g_strjoin(G_DIR_SEPARATOR_S, path, name, NULL);

	// determine new relative path
	if (relpath == NULL)
	    newrelpath = strdup(name);
	else
	    newrelpath = g_strjoin(G_DIR_SEPARATOR_S, relpath, name, NULL);


	// obtain file info
	if ((ret = stat(newpath, &st)) != 0) {
    	    g_set_error(err, SCS_ERROR, SCS_ERROR_SYS, "could not stat: %s", newpath);
	    goto fail;
	}

	if (S_ISDIR(st.st_mode)) {
	    if (recursive) {
		// recurse into directories
		if ((list = _sys_scandir(list, newpath, newrelpath, recursive, &tmp)) == NULL) {
		    g_propagate_error(err, tmp);
		    goto fail;
		}

		g_free(newrelpath);
	    }
	} else {
	    // append filename to list
	    file = old_file_new((char *)newrelpath);
	    file->size = st.st_size;

	    list = g_list_append(list, file);
	}
    }

    g_free(newrelpath);
    g_free(newpath);
    return list;

fail:
    g_free(newrelpath);
    g_free(newpath);
    return NULL;
}

GList *
sys_scandir(char *path, gboolean recursive, GError **err)
{
    GError *tmp = NULL;
    GList *list = NULL;

    if ((list = _sys_scandir(list, path, NULL, recursive, &tmp)) == NULL) {
	g_propagate_error(err, tmp);
	return NULL;
    }

    return list;
}
