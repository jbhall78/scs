#include <stdlib.h>

#include <glib.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "shared.h"

/* placed here to avoid dependency loop in headers */
void gl_throw_error(char *msg);
void gl_clear_error(void);
char *gl_strerror(GLenum error);

/*
 * later we can add errors to a list and check/display them each frame
 */
void
gl_throw_error(char *msg)
{
    printf("%s\n", msg);
    free(msg);
}

void
gl_clear_error(void)
{
    glGetError();
}

char *
gl_strerror(GLenum error)
{
    switch (error) {
        case GL_NO_ERROR:
            return "No Error";
        case GL_INVALID_ENUM:
            return "Invalid Enum";
        case GL_INVALID_VALUE:
            return "Invalid Value";
        case GL_INVALID_OPERATION:
            return "Invalid Operation";
        case GL_STACK_OVERFLOW:
            return "Stack Overflow";
        case GL_STACK_UNDERFLOW:
            return "Stack Underflow";
        case GL_OUT_OF_MEMORY:
            return "Out Of Memory";
        case GL_TABLE_TOO_LARGE:
            return "Table Too Large";
        default:
            return "Unknown Error";
    }
}

/* add a hint to the error message */
void
CUSTOM_glTexImage2D(
	const char *filename, const char *function, int line,
	GLenum target, GLint level,
	GLint internalFormat,
	GLsizei width, GLsizei height,
	GLint border, GLenum format, GLenum type,
	const GLvoid *pixels
)
{
    GLenum error;
    char *hint = " ";

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif

    glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels);

    error = glGetError();
    if (error == 0)
	return;

    if (width != height)
	hint = "(width != height)? ";

#ifdef DEBUG
    print("glTexImage2D(0x%x, %d, 0x%x, %d, %d, %d, 0x%x, 0x%x, 0x%08x)\n",
	    target, level, internalFormat, width, height, border, format, type, pixels);
#endif

    gl_throw_error(mkstr("[%s::%s:%d]: OpenGL glTexImage2D rejected texture: %s %s",
	    filename, function, line, gl_strerror(error), hint));
}

/*
inline void
OpenGL_glBegin(const char *filename, const char *function, int line, GLenum mode)
{
    extern GLboolean DisableOpenGLErrorChecking;

    glBegin(mode);

    DisableOpenGLErrorChecking = TRUE;

    return;
}

inline void
OpenGL_glEnd(const char *filename, const char *function, int line)
{
    extern GLboolean DisableOpenGLErrorChecking;

    glEnd();

    DisableOpenGLErrorChecking = FALSE;

    return;
}
*/



/* glEnd() returns errors for no reason apparently
inline void
CUSTOM_glEnd(const char *filename, const char *function, int line)
{
    glEnd();
    return;
}
*/
