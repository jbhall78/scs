#ifndef _SYS_GL_H
#define _SYS_GL_H

#include <glib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "shared.h"
#define CLEAR_ERRORS 1
#include "sys_gl_auto.h"

/*
typedef struct {
    void PixelStorei(
            const char *filename, const char *function, int line,
	    GLenum pname, GLint param );
    void (*GenTextures)(
	    const char *filename, const char *function, int line,
	    GLsizei n, GLuint *textures );

    void (*BindTexture)(
	    const char *filename, const char *function, int line,
	    GLenum target, GLuint texture);

    void (*TexImage2D)(
    	    const char *filename, const char *function, int line,
	    GLenum target, GLint level,
	    GLint internalFormat,
	    GLsizei width, GLsizei height,
	    GLint border, GLenum format, GLenum type,
	    const GLvoid *pixels);
    void (*init)(void);
} gl_funcs_t;

#define PixelStorei(pname, param) \
    PixelStorei( \
	    __FILE__,FUNCTION,__LINE__ \
	    pname, param)

#define GenTextures(n, textures) \
    GenTextures( \
	    __FILE__,FUNCTION,__LINE__ \
	    n, textures)

#define BindTexture(GLenum target, GLuint texture) \
    BindTexture( \
	    __FILE__,FUNCTION,__LINE__ \
	    GLenum target, GLuint texture)

#define TexImage2D(target,level,internalFormat,width,height,border,format,type,pixels) \
    TexImage2D( \
	    __FILE__,FUNCTION,__LINE__ \
	    target,level,internalFormat,width,height,border,format,type,pixels)
*/

void CUSTOM_glTexImage2D(
	const char *filename, const char *function, int line,
	GLenum target, GLint level,
	GLint internalFormat,
	GLsizei width, GLsizei height,
	GLint border, GLenum format, GLenum type,
	const GLvoid *pixels);
//void CUSTOM_glEnd(const char *filename, const char *function, int line);


/*
void OpenGL_init(void);
	    
gl_funcs_t _gl;
gl_funcs_t *gl;
*/

#ifndef DEBUG
#undef glTexImage2D
#define glTexImage2D(target,level,internalFormat,width,height,border,format,type,pixels) CUSTOM_glTexImage2D(__FILE__,FUNCTION,__LINE__,target,level,internalFormat,width,height,border,format,type,pixels)
#endif
//#undef glEnd
//#define glEnd(void) CUSTOM_glEnd(__FILE__,FUNCTION,__LINE__)

#ifdef MATH64
# define glMultMatrix   glMultMatrixd
# define glColor3v      glColor3dv
# define glColor4v      glColor4dv
# define glNormal3v     glNormal3dv
# define glVertex2      glVertex2d
# define glVertex3v     glVertex3dv
# define glTexCoord2    glTexCoord2d
# define glTranslate    glTranslated
# define glTexCoord2v   glTexCoord2dv
# define glGetv         glGetDoublev
#else
# define glMultMatrix   glMultMatrixf
# define glColor3v      glColor3fv
# define glColor4v      glColor4fv
# define glNormal3v     glNormal3fv
# define glVertex2      glVertex2f
# define glVertex3v     glVertex3fv
# define glTexCoord2    glTexCoord2f
# define glTranslate    glTranslatef
# define glTexCoord2v   glTexCoord2fv
# define glGetv         glGetFloatv
#endif


#endif
