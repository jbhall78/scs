#include <GL/gl.h>
#include <GL/glu.h>

#include "shared.h"

/* placed here to avoid dependency loop in headers */
void gl_throw_error(char *msg);
void gl_clear_error(void);
char *gl_strerror(GLenum error);

void
OpenGL_glAccum(const char *filename, const char *function, int line, GLenum op, GLfloat value)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glAccum(op,value);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glAccum: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glActiveTexture(const char *filename, const char *function, int line, GLenum texture)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glActiveTexture(texture);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glActiveTexture: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glActiveTextureARB(const char *filename, const char *function, int line, GLenum texture)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glActiveTextureARB(texture);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glActiveTextureARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glAlphaFunc(const char *filename, const char *function, int line, GLenum func, GLclampf ref)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glAlphaFunc(func,ref);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glAlphaFunc: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLboolean
OpenGL_glAreTexturesResident(const char *filename, const char *function, int line, GLsizei n, const GLuint *textures, GLboolean *residences)
{
    GLenum glerror;
    GLboolean retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = glAreTexturesResident(n,textures,residences);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glAreTexturesResident: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_glArrayElement(const char *filename, const char *function, int line, GLint i)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glArrayElement(i);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glArrayElement: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glBindTexture(const char *filename, const char *function, int line, GLenum target, GLuint texture)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glBindTexture(target,texture);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glBindTexture: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glBitmap(const char *filename, const char *function, int line, GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glBitmap(width,height,xorig,yorig,xmove,ymove,bitmap);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glBitmap: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glBlendColor(const char *filename, const char *function, int line, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glBlendColor(red,green,blue,alpha);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glBlendColor: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glBlendEquation(const char *filename, const char *function, int line, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glBlendEquation(mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glBlendEquation: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glBlendFunc(const char *filename, const char *function, int line, GLenum sfactor, GLenum dfactor)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glBlendFunc(sfactor,dfactor);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glBlendFunc: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCallList(const char *filename, const char *function, int line, GLuint list)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCallList(list);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCallList: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCallLists(const char *filename, const char *function, int line, GLsizei n, GLenum type, const GLvoid *lists)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCallLists(n,type,lists);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCallLists: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClear(const char *filename, const char *function, int line, GLbitfield mask)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClear(mask);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClear: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClearAccum(const char *filename, const char *function, int line, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClearAccum(red,green,blue,alpha);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClearAccum: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClearColor(const char *filename, const char *function, int line, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClearColor(red,green,blue,alpha);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClearColor: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClearDepth(const char *filename, const char *function, int line, GLclampd depth)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClearDepth(depth);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClearDepth: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClearIndex(const char *filename, const char *function, int line, GLfloat c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClearIndex(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClearIndex: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClearStencil(const char *filename, const char *function, int line, GLint s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClearStencil(s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClearStencil: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClientActiveTexture(const char *filename, const char *function, int line, GLenum texture)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClientActiveTexture(texture);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClientActiveTexture: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClientActiveTextureARB(const char *filename, const char *function, int line, GLenum texture)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClientActiveTextureARB(texture);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClientActiveTextureARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glClipPlane(const char *filename, const char *function, int line, GLenum plane, const GLdouble *equation)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glClipPlane(plane,equation);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glClipPlane: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCompressedTexImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCompressedTexImage1D(target,level,internalformat,width,border,imageSize,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCompressedTexImage1D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCompressedTexImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCompressedTexImage2D(target,level,internalformat,width,height,border,imageSize,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCompressedTexImage2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCompressedTexImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCompressedTexImage3D(target,level,internalformat,width,height,depth,border,imageSize,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCompressedTexImage3D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCompressedTexSubImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCompressedTexSubImage1D(target,level,xoffset,width,format,imageSize,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCompressedTexSubImage1D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCompressedTexSubImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCompressedTexSubImage2D(target,level,xoffset,yoffset,width,height,format,imageSize,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCompressedTexSubImage2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCompressedTexSubImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCompressedTexSubImage3D(target,level,xoffset,yoffset,zoffset,width,height,depth,format,imageSize,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCompressedTexSubImage3D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glConvolutionFilter1D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glConvolutionFilter1D(target,internalformat,width,format,type,image);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glConvolutionFilter1D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glConvolutionFilter2D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glConvolutionFilter2D(target,internalformat,width,height,format,type,image);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glConvolutionFilter2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glConvolutionParameterf(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glConvolutionParameterf(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glConvolutionParameterf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glConvolutionParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glConvolutionParameterfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glConvolutionParameterfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glConvolutionParameteri(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glConvolutionParameteri(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glConvolutionParameteri: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glConvolutionParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glConvolutionParameteriv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glConvolutionParameteriv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyColorSubTable(const char *filename, const char *function, int line, GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyColorSubTable(target,start,x,y,width);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyColorSubTable: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyColorTable(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyColorTable(target,internalformat,x,y,width);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyColorTable: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyConvolutionFilter1D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyConvolutionFilter1D(target,internalformat,x,y,width);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyConvolutionFilter1D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyConvolutionFilter2D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyConvolutionFilter2D(target,internalformat,x,y,width,height);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyConvolutionFilter2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyPixels(const char *filename, const char *function, int line, GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyPixels(x,y,width,height,type);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyPixels: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyTexImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyTexImage1D(target,level,internalformat,x,y,width,border);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyTexImage1D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyTexImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyTexImage2D(target,level,internalformat,x,y,width,height,border);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyTexImage2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyTexSubImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyTexSubImage1D(target,level,xoffset,x,y,width);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyTexSubImage1D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyTexSubImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyTexSubImage2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCopyTexSubImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCopyTexSubImage3D(target,level,xoffset,yoffset,zoffset,x,y,width,height);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCopyTexSubImage3D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glCullFace(const char *filename, const char *function, int line, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glCullFace(mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glCullFace: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDeleteLists(const char *filename, const char *function, int line, GLuint list, GLsizei range)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDeleteLists(list,range);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDeleteLists: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDeleteTextures(const char *filename, const char *function, int line, GLsizei n, const GLuint *textures)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDeleteTextures(n,textures);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDeleteTextures: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDepthFunc(const char *filename, const char *function, int line, GLenum func)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDepthFunc(func);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDepthFunc: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDepthMask(const char *filename, const char *function, int line, GLboolean flag)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDepthMask(flag);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDepthMask: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDepthRange(const char *filename, const char *function, int line, GLclampd near_val, GLclampd far_val)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDepthRange(near_val,far_val);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDepthRange: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDisable(const char *filename, const char *function, int line, GLenum cap)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDisable(cap);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDisable: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDisableClientState(const char *filename, const char *function, int line, GLenum cap)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDisableClientState(cap);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDisableClientState: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDrawArrays(const char *filename, const char *function, int line, GLenum mode, GLint first, GLsizei count)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDrawArrays(mode,first,count);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDrawArrays: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDrawBuffer(const char *filename, const char *function, int line, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDrawBuffer(mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDrawBuffer: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDrawElements(const char *filename, const char *function, int line, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDrawElements(mode,count,type,indices);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDrawElements: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDrawPixels(const char *filename, const char *function, int line, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDrawPixels(width,height,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDrawPixels: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glDrawRangeElements(const char *filename, const char *function, int line, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glDrawRangeElements(mode,start,end,count,type,indices);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glDrawRangeElements: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEdgeFlag(const char *filename, const char *function, int line, GLboolean flag)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEdgeFlag(flag);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEdgeFlag: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEdgeFlagPointer(const char *filename, const char *function, int line, GLsizei stride, const GLvoid *ptr)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEdgeFlagPointer(stride,ptr);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEdgeFlagPointer: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEdgeFlagv(const char *filename, const char *function, int line, const GLboolean *flag)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEdgeFlagv(flag);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEdgeFlagv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEnable(const char *filename, const char *function, int line, GLenum cap)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEnable(cap);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEnable: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEnableClientState(const char *filename, const char *function, int line, GLenum cap)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEnableClientState(cap);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEnableClientState: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalCoord1d(const char *filename, const char *function, int line, GLdouble u)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalCoord1d(u);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalCoord1d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalCoord1dv(const char *filename, const char *function, int line, const GLdouble *u)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalCoord1dv(u);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalCoord1dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalCoord1f(const char *filename, const char *function, int line, GLfloat u)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalCoord1f(u);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalCoord1f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalCoord1fv(const char *filename, const char *function, int line, const GLfloat *u)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalCoord1fv(u);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalCoord1fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalCoord2d(const char *filename, const char *function, int line, GLdouble u, GLdouble v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalCoord2d(u,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalCoord2d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalCoord2dv(const char *filename, const char *function, int line, const GLdouble *u)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalCoord2dv(u);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalCoord2dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalCoord2f(const char *filename, const char *function, int line, GLfloat u, GLfloat v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalCoord2f(u,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalCoord2f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalCoord2fv(const char *filename, const char *function, int line, const GLfloat *u)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalCoord2fv(u);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalCoord2fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalMesh1(const char *filename, const char *function, int line, GLenum mode, GLint i1, GLint i2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalMesh1(mode,i1,i2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalMesh1: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalMesh2(const char *filename, const char *function, int line, GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalMesh2(mode,i1,i2,j1,j2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalMesh2: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalPoint1(const char *filename, const char *function, int line, GLint i)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalPoint1(i);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalPoint1: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glEvalPoint2(const char *filename, const char *function, int line, GLint i, GLint j)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glEvalPoint2(i,j);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glEvalPoint2: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFeedbackBuffer(const char *filename, const char *function, int line, GLsizei size, GLenum type, GLfloat *buffer)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFeedbackBuffer(size,type,buffer);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFeedbackBuffer: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFinish(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFinish();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFinish: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFlush(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFlush();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFlush: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFogf(const char *filename, const char *function, int line, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFogf(pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFogf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFogfv(const char *filename, const char *function, int line, GLenum pname, const GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFogfv(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFogfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFogi(const char *filename, const char *function, int line, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFogi(pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFogi: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFogiv(const char *filename, const char *function, int line, GLenum pname, const GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFogiv(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFogiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFrontFace(const char *filename, const char *function, int line, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFrontFace(mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFrontFace: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glFrustum(const char *filename, const char *function, int line, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glFrustum(left,right,bottom,top,near_val,far_val);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glFrustum: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLuint
OpenGL_glGenLists(const char *filename, const char *function, int line, GLsizei range)
{
    GLenum glerror;
    GLuint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = glGenLists(range);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGenLists: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_glGenTextures(const char *filename, const char *function, int line, GLsizei n, GLuint *textures)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGenTextures(n,textures);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGenTextures: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetBooleanv(const char *filename, const char *function, int line, GLenum pname, GLboolean *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetBooleanv(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetBooleanv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetClipPlane(const char *filename, const char *function, int line, GLenum plane, GLdouble *equation)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetClipPlane(plane,equation);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetClipPlane: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetColorTable(const char *filename, const char *function, int line, GLenum target, GLenum format, GLenum type, GLvoid *table)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetColorTable(target,format,type,table);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetColorTable: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetColorTableParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetColorTableParameterfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetColorTableParameterfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetColorTableParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetColorTableParameteriv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetColorTableParameteriv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetCompressedTexImage(const char *filename, const char *function, int line, GLenum target, GLint lod, GLvoid *img)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetCompressedTexImage(target,lod,img);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetCompressedTexImage: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetConvolutionFilter(const char *filename, const char *function, int line, GLenum target, GLenum format, GLenum type, GLvoid *image)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetConvolutionFilter(target,format,type,image);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetConvolutionFilter: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetConvolutionParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetConvolutionParameterfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetConvolutionParameterfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetConvolutionParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetConvolutionParameteriv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetConvolutionParameteriv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetDoublev(const char *filename, const char *function, int line, GLenum pname, GLdouble *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetDoublev(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetDoublev: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLenum
OpenGL_glGetError(const char *filename, const char *function, int line)
{
    GLenum glerror;
    GLenum retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = glGetError();

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetError: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_glGetFloatv(const char *filename, const char *function, int line, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetFloatv(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetFloatv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetHistogram(const char *filename, const char *function, int line, GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetHistogram(target,reset,format,type,values);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetHistogram: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetHistogramParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetHistogramParameterfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetHistogramParameterfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetHistogramParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetHistogramParameteriv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetHistogramParameteriv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetIntegerv(const char *filename, const char *function, int line, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetIntegerv(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetIntegerv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetLightfv(const char *filename, const char *function, int line, GLenum light, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetLightfv(light,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetLightfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetLightiv(const char *filename, const char *function, int line, GLenum light, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetLightiv(light,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetLightiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetMapdv(const char *filename, const char *function, int line, GLenum target, GLenum query, GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetMapdv(target,query,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetMapdv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetMapfv(const char *filename, const char *function, int line, GLenum target, GLenum query, GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetMapfv(target,query,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetMapfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetMapiv(const char *filename, const char *function, int line, GLenum target, GLenum query, GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetMapiv(target,query,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetMapiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetMaterialfv(const char *filename, const char *function, int line, GLenum face, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetMaterialfv(face,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetMaterialfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetMaterialiv(const char *filename, const char *function, int line, GLenum face, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetMaterialiv(face,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetMaterialiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetMinmax(const char *filename, const char *function, int line, GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetMinmax(target,reset,format,types,values);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetMinmax: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetMinmaxParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetMinmaxParameterfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetMinmaxParameterfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetMinmaxParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetMinmaxParameteriv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetMinmaxParameteriv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetPixelMapfv(const char *filename, const char *function, int line, GLenum map, GLfloat *values)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetPixelMapfv(map,values);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetPixelMapfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetPixelMapuiv(const char *filename, const char *function, int line, GLenum map, GLuint *values)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetPixelMapuiv(map,values);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetPixelMapuiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetPixelMapusv(const char *filename, const char *function, int line, GLenum map, GLushort *values)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetPixelMapusv(map,values);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetPixelMapusv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetPointerv(const char *filename, const char *function, int line, GLenum pname, GLvoid **params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetPointerv(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetPointerv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetPolygonStipple(const char *filename, const char *function, int line, GLubyte *mask)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetPolygonStipple(mask);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetPolygonStipple: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetSeparableFilter(const char *filename, const char *function, int line, GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetSeparableFilter(target,format,type,row,column,span);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetSeparableFilter: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

const GLubyte *
OpenGL_glGetString(const char *filename, const char *function, int line, GLenum name)
{
    GLenum glerror;
    const GLubyte * retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = glGetString(name);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetString: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_glGetTexEnvfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexEnvfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexEnvfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexEnviv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexEnviv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexEnviv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexGendv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLdouble *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexGendv(coord,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexGendv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexGenfv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexGenfv(coord,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexGenfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexGeniv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexGeniv(coord,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexGeniv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexImage(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexImage(target,level,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexImage: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexLevelParameterfv(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexLevelParameterfv(target,level,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexLevelParameterfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexLevelParameteriv(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexLevelParameteriv(target,level,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexLevelParameteriv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexParameterfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexParameterfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glGetTexParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glGetTexParameteriv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glGetTexParameteriv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glHint(const char *filename, const char *function, int line, GLenum target, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glHint(target,mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glHint: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glHistogram(const char *filename, const char *function, int line, GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glHistogram(target,width,internalformat,sink);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glHistogram: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexMask(const char *filename, const char *function, int line, GLuint mask)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexMask(mask);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexMask: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexPointer(const char *filename, const char *function, int line, GLenum type, GLsizei stride, const GLvoid *ptr)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexPointer(type,stride,ptr);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexPointer: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexd(const char *filename, const char *function, int line, GLdouble c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexd(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexd: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexdv(const char *filename, const char *function, int line, const GLdouble *c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexdv(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexdv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexf(const char *filename, const char *function, int line, GLfloat c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexf(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexfv(const char *filename, const char *function, int line, const GLfloat *c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexfv(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexi(const char *filename, const char *function, int line, GLint c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexi(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexi: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexiv(const char *filename, const char *function, int line, const GLint *c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexiv(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexs(const char *filename, const char *function, int line, GLshort c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexs(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexs: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexsv(const char *filename, const char *function, int line, const GLshort *c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexsv(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexsv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexub(const char *filename, const char *function, int line, GLubyte c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexub(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexub: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glIndexubv(const char *filename, const char *function, int line, const GLubyte *c)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glIndexubv(c);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIndexubv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glInitNames(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glInitNames();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glInitNames: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glInterleavedArrays(const char *filename, const char *function, int line, GLenum format, GLsizei stride, const GLvoid *pointer)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glInterleavedArrays(format,stride,pointer);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glInterleavedArrays: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLboolean
OpenGL_glIsEnabled(const char *filename, const char *function, int line, GLenum cap)
{
    GLenum glerror;
    GLboolean retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = glIsEnabled(cap);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIsEnabled: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLboolean
OpenGL_glIsList(const char *filename, const char *function, int line, GLuint list)
{
    GLenum glerror;
    GLboolean retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = glIsList(list);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIsList: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLboolean
OpenGL_glIsTexture(const char *filename, const char *function, int line, GLuint texture)
{
    GLenum glerror;
    GLboolean retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = glIsTexture(texture);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glIsTexture: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_glLightModelf(const char *filename, const char *function, int line, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLightModelf(pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLightModelf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLightModelfv(const char *filename, const char *function, int line, GLenum pname, const GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLightModelfv(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLightModelfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLightModeli(const char *filename, const char *function, int line, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLightModeli(pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLightModeli: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLightModeliv(const char *filename, const char *function, int line, GLenum pname, const GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLightModeliv(pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLightModeliv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLightf(const char *filename, const char *function, int line, GLenum light, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLightf(light,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLightf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLightfv(const char *filename, const char *function, int line, GLenum light, GLenum pname, const GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLightfv(light,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLightfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLighti(const char *filename, const char *function, int line, GLenum light, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLighti(light,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLighti: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLightiv(const char *filename, const char *function, int line, GLenum light, GLenum pname, const GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLightiv(light,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLightiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLineStipple(const char *filename, const char *function, int line, GLint factor, GLushort pattern)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLineStipple(factor,pattern);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLineStipple: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLineWidth(const char *filename, const char *function, int line, GLfloat width)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLineWidth(width);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLineWidth: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glListBase(const char *filename, const char *function, int line, GLuint base)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glListBase(base);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glListBase: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLoadIdentity(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLoadIdentity();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLoadIdentity: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLoadMatrixd(const char *filename, const char *function, int line, const GLdouble *m)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLoadMatrixd(m);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLoadMatrixd: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLoadMatrixf(const char *filename, const char *function, int line, const GLfloat *m)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLoadMatrixf(m);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLoadMatrixf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLoadName(const char *filename, const char *function, int line, GLuint name)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLoadName(name);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLoadName: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLoadTransposeMatrixd(const char *filename, const char *function, int line, const GLdouble m[16])
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLoadTransposeMatrixd(m);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLoadTransposeMatrixd: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLoadTransposeMatrixf(const char *filename, const char *function, int line, const GLfloat m[16])
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLoadTransposeMatrixf(m);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLoadTransposeMatrixf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glLogicOp(const char *filename, const char *function, int line, GLenum opcode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glLogicOp(opcode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glLogicOp: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMap1d(const char *filename, const char *function, int line, GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMap1d(target,u1,u2,stride,order,points);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMap1d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMap1f(const char *filename, const char *function, int line, GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMap1f(target,u1,u2,stride,order,points);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMap1f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMap2d(const char *filename, const char *function, int line, GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMap2d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMap2f(const char *filename, const char *function, int line, GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMap2f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMapGrid1d(const char *filename, const char *function, int line, GLint un, GLdouble u1, GLdouble u2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMapGrid1d(un,u1,u2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMapGrid1d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMapGrid1f(const char *filename, const char *function, int line, GLint un, GLfloat u1, GLfloat u2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMapGrid1f(un,u1,u2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMapGrid1f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMapGrid2d(const char *filename, const char *function, int line, GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMapGrid2d(un,u1,u2,vn,v1,v2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMapGrid2d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMapGrid2f(const char *filename, const char *function, int line, GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMapGrid2f(un,u1,u2,vn,v1,v2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMapGrid2f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMaterialf(const char *filename, const char *function, int line, GLenum face, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMaterialf(face,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMaterialf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMaterialfv(const char *filename, const char *function, int line, GLenum face, GLenum pname, const GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMaterialfv(face,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMaterialfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMateriali(const char *filename, const char *function, int line, GLenum face, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMateriali(face,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMateriali: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMaterialiv(const char *filename, const char *function, int line, GLenum face, GLenum pname, const GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMaterialiv(face,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMaterialiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMatrixMode(const char *filename, const char *function, int line, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMatrixMode(mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMatrixMode: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMinmax(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLboolean sink)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMinmax(target,internalformat,sink);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMinmax: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultMatrixd(const char *filename, const char *function, int line, const GLdouble *m)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultMatrixd(m);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultMatrixd: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultMatrixf(const char *filename, const char *function, int line, const GLfloat *m)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultMatrixf(m);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultMatrixf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultTransposeMatrixd(const char *filename, const char *function, int line, const GLdouble m[16])
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultTransposeMatrixd(m);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultTransposeMatrixd: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultTransposeMatrixf(const char *filename, const char *function, int line, const GLfloat m[16])
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultTransposeMatrixf(m);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultTransposeMatrixf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1d(const char *filename, const char *function, int line, GLenum target, GLdouble s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1d(target,s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1dARB(const char *filename, const char *function, int line, GLenum target, GLdouble s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1dARB(target,s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1dARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1dv(const char *filename, const char *function, int line, GLenum target, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1dv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1dvARB(const char *filename, const char *function, int line, GLenum target, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1dvARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1dvARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1f(const char *filename, const char *function, int line, GLenum target, GLfloat s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1f(target,s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1fARB(const char *filename, const char *function, int line, GLenum target, GLfloat s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1fARB(target,s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1fARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1fv(const char *filename, const char *function, int line, GLenum target, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1fv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1fvARB(const char *filename, const char *function, int line, GLenum target, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1fvARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1fvARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1i(const char *filename, const char *function, int line, GLenum target, GLint s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1i(target,s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1i: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1iARB(const char *filename, const char *function, int line, GLenum target, GLint s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1iARB(target,s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1iARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1iv(const char *filename, const char *function, int line, GLenum target, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1iv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1iv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1ivARB(const char *filename, const char *function, int line, GLenum target, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1ivARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1ivARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1s(const char *filename, const char *function, int line, GLenum target, GLshort s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1s(target,s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1s: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1sARB(const char *filename, const char *function, int line, GLenum target, GLshort s)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1sARB(target,s);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1sARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1sv(const char *filename, const char *function, int line, GLenum target, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1sv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1sv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord1svARB(const char *filename, const char *function, int line, GLenum target, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord1svARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord1svARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2d(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2d(target,s,t);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2dARB(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2dARB(target,s,t);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2dARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2dv(const char *filename, const char *function, int line, GLenum target, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2dv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2dvARB(const char *filename, const char *function, int line, GLenum target, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2dvARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2dvARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2f(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2f(target,s,t);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2fARB(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2fARB(target,s,t);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2fARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2fv(const char *filename, const char *function, int line, GLenum target, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2fv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2fvARB(const char *filename, const char *function, int line, GLenum target, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2fvARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2fvARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2i(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2i(target,s,t);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2i: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2iARB(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2iARB(target,s,t);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2iARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2iv(const char *filename, const char *function, int line, GLenum target, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2iv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2iv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2ivARB(const char *filename, const char *function, int line, GLenum target, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2ivARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2ivARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2s(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2s(target,s,t);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2s: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2sARB(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2sARB(target,s,t);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2sARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2sv(const char *filename, const char *function, int line, GLenum target, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2sv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2sv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord2svARB(const char *filename, const char *function, int line, GLenum target, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord2svARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord2svARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3d(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t, GLdouble r)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3d(target,s,t,r);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3dARB(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t, GLdouble r)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3dARB(target,s,t,r);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3dARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3dv(const char *filename, const char *function, int line, GLenum target, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3dv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3dvARB(const char *filename, const char *function, int line, GLenum target, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3dvARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3dvARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3f(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t, GLfloat r)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3f(target,s,t,r);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3fARB(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t, GLfloat r)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3fARB(target,s,t,r);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3fARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3fv(const char *filename, const char *function, int line, GLenum target, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3fv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3fvARB(const char *filename, const char *function, int line, GLenum target, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3fvARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3fvARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3i(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t, GLint r)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3i(target,s,t,r);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3i: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3iARB(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t, GLint r)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3iARB(target,s,t,r);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3iARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3iv(const char *filename, const char *function, int line, GLenum target, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3iv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3iv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3ivARB(const char *filename, const char *function, int line, GLenum target, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3ivARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3ivARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3s(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t, GLshort r)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3s(target,s,t,r);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3s: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3sARB(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t, GLshort r)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3sARB(target,s,t,r);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3sARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3sv(const char *filename, const char *function, int line, GLenum target, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3sv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3sv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord3svARB(const char *filename, const char *function, int line, GLenum target, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord3svARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord3svARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4d(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4d(target,s,t,r,q);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4dARB(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4dARB(target,s,t,r,q);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4dARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4dv(const char *filename, const char *function, int line, GLenum target, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4dv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4dvARB(const char *filename, const char *function, int line, GLenum target, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4dvARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4dvARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4f(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4f(target,s,t,r,q);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4fARB(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4fARB(target,s,t,r,q);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4fARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4fv(const char *filename, const char *function, int line, GLenum target, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4fv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4fvARB(const char *filename, const char *function, int line, GLenum target, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4fvARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4fvARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4i(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t, GLint r, GLint q)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4i(target,s,t,r,q);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4i: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4iARB(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t, GLint r, GLint q)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4iARB(target,s,t,r,q);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4iARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4iv(const char *filename, const char *function, int line, GLenum target, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4iv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4iv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4ivARB(const char *filename, const char *function, int line, GLenum target, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4ivARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4ivARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4s(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4s(target,s,t,r,q);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4s: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4sARB(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4sARB(target,s,t,r,q);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4sARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4sv(const char *filename, const char *function, int line, GLenum target, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4sv(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4sv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glMultiTexCoord4svARB(const char *filename, const char *function, int line, GLenum target, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glMultiTexCoord4svARB(target,v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glMultiTexCoord4svARB: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glNewList(const char *filename, const char *function, int line, GLuint list, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glNewList(list,mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glNewList: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glOrtho(const char *filename, const char *function, int line, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glOrtho(left,right,bottom,top,near_val,far_val);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glOrtho: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPassThrough(const char *filename, const char *function, int line, GLfloat token)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPassThrough(token);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPassThrough: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPixelMapfv(const char *filename, const char *function, int line, GLenum map, GLsizei mapsize, const GLfloat *values)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPixelMapfv(map,mapsize,values);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPixelMapfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPixelMapuiv(const char *filename, const char *function, int line, GLenum map, GLsizei mapsize, const GLuint *values)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPixelMapuiv(map,mapsize,values);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPixelMapuiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPixelMapusv(const char *filename, const char *function, int line, GLenum map, GLsizei mapsize, const GLushort *values)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPixelMapusv(map,mapsize,values);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPixelMapusv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPixelStoref(const char *filename, const char *function, int line, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPixelStoref(pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPixelStoref: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPixelStorei(const char *filename, const char *function, int line, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPixelStorei(pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPixelStorei: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPixelTransferf(const char *filename, const char *function, int line, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPixelTransferf(pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPixelTransferf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPixelTransferi(const char *filename, const char *function, int line, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPixelTransferi(pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPixelTransferi: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPixelZoom(const char *filename, const char *function, int line, GLfloat xfactor, GLfloat yfactor)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPixelZoom(xfactor,yfactor);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPixelZoom: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPointSize(const char *filename, const char *function, int line, GLfloat size)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPointSize(size);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPointSize: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPolygonMode(const char *filename, const char *function, int line, GLenum face, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPolygonMode(face,mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPolygonMode: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPolygonOffset(const char *filename, const char *function, int line, GLfloat factor, GLfloat units)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPolygonOffset(factor,units);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPolygonOffset: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPolygonStipple(const char *filename, const char *function, int line, const GLubyte *mask)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPolygonStipple(mask);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPolygonStipple: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPopAttrib(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPopAttrib();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPopAttrib: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPopClientAttrib(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPopClientAttrib();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPopClientAttrib: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPopMatrix(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPopMatrix();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPopMatrix: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPopName(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPopName();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPopName: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPrioritizeTextures(const char *filename, const char *function, int line, GLsizei n, const GLuint *textures, const GLclampf *priorities)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPrioritizeTextures(n,textures,priorities);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPrioritizeTextures: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPushAttrib(const char *filename, const char *function, int line, GLbitfield mask)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPushAttrib(mask);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPushAttrib: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPushClientAttrib(const char *filename, const char *function, int line, GLbitfield mask)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPushClientAttrib(mask);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPushClientAttrib: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPushMatrix(const char *filename, const char *function, int line)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPushMatrix();

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPushMatrix: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glPushName(const char *filename, const char *function, int line, GLuint name)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glPushName(name);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glPushName: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos2d(const char *filename, const char *function, int line, GLdouble x, GLdouble y)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos2d(x,y);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos2d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos2dv(const char *filename, const char *function, int line, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos2dv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos2dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos2f(const char *filename, const char *function, int line, GLfloat x, GLfloat y)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos2f(x,y);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos2f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos2fv(const char *filename, const char *function, int line, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos2fv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos2fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos2i(const char *filename, const char *function, int line, GLint x, GLint y)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos2i(x,y);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos2i: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos2iv(const char *filename, const char *function, int line, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos2iv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos2iv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos2s(const char *filename, const char *function, int line, GLshort x, GLshort y)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos2s(x,y);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos2s: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos2sv(const char *filename, const char *function, int line, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos2sv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos2sv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos3d(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos3d(x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos3d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos3dv(const char *filename, const char *function, int line, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos3dv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos3dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos3f(const char *filename, const char *function, int line, GLfloat x, GLfloat y, GLfloat z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos3f(x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos3f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos3fv(const char *filename, const char *function, int line, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos3fv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos3fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos3i(const char *filename, const char *function, int line, GLint x, GLint y, GLint z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos3i(x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos3i: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos3iv(const char *filename, const char *function, int line, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos3iv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos3iv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos3s(const char *filename, const char *function, int line, GLshort x, GLshort y, GLshort z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos3s(x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos3s: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos3sv(const char *filename, const char *function, int line, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos3sv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos3sv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos4d(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos4d(x,y,z,w);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos4d: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos4dv(const char *filename, const char *function, int line, const GLdouble *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos4dv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos4dv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos4f(const char *filename, const char *function, int line, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos4f(x,y,z,w);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos4f: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos4fv(const char *filename, const char *function, int line, const GLfloat *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos4fv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos4fv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos4i(const char *filename, const char *function, int line, GLint x, GLint y, GLint z, GLint w)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos4i(x,y,z,w);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos4i: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos4iv(const char *filename, const char *function, int line, const GLint *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos4iv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos4iv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos4s(const char *filename, const char *function, int line, GLshort x, GLshort y, GLshort z, GLshort w)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos4s(x,y,z,w);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos4s: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRasterPos4sv(const char *filename, const char *function, int line, const GLshort *v)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRasterPos4sv(v);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRasterPos4sv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glReadBuffer(const char *filename, const char *function, int line, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glReadBuffer(mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glReadBuffer: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glReadPixels(const char *filename, const char *function, int line, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glReadPixels(x,y,width,height,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glReadPixels: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRectd(const char *filename, const char *function, int line, GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRectd(x1,y1,x2,y2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRectd: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRectdv(const char *filename, const char *function, int line, const GLdouble *v1, const GLdouble *v2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRectdv(v1,v2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRectdv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRectf(const char *filename, const char *function, int line, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRectf(x1,y1,x2,y2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRectf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRectfv(const char *filename, const char *function, int line, const GLfloat *v1, const GLfloat *v2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRectfv(v1,v2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRectfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRecti(const char *filename, const char *function, int line, GLint x1, GLint y1, GLint x2, GLint y2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRecti(x1,y1,x2,y2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRecti: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRectiv(const char *filename, const char *function, int line, const GLint *v1, const GLint *v2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRectiv(v1,v2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRectiv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRects(const char *filename, const char *function, int line, GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRects(x1,y1,x2,y2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRects: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRectsv(const char *filename, const char *function, int line, const GLshort *v1, const GLshort *v2)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRectsv(v1,v2);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRectsv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLint
OpenGL_glRenderMode(const char *filename, const char *function, int line, GLenum mode)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = glRenderMode(mode);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRenderMode: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_glResetHistogram(const char *filename, const char *function, int line, GLenum target)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glResetHistogram(target);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glResetHistogram: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glResetMinmax(const char *filename, const char *function, int line, GLenum target)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glResetMinmax(target);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glResetMinmax: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRotated(const char *filename, const char *function, int line, GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRotated(angle,x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRotated: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glRotatef(const char *filename, const char *function, int line, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glRotatef(angle,x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glRotatef: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glSampleCoverage(const char *filename, const char *function, int line, GLclampf value, GLboolean invert)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glSampleCoverage(value,invert);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glSampleCoverage: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glScaled(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glScaled(x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glScaled: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glScalef(const char *filename, const char *function, int line, GLfloat x, GLfloat y, GLfloat z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glScalef(x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glScalef: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glScissor(const char *filename, const char *function, int line, GLint x, GLint y, GLsizei width, GLsizei height)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glScissor(x,y,width,height);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glScissor: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glSelectBuffer(const char *filename, const char *function, int line, GLsizei size, GLuint *buffer)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glSelectBuffer(size,buffer);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glSelectBuffer: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glSeparableFilter2D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glSeparableFilter2D(target,internalformat,width,height,format,type,row,column);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glSeparableFilter2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glShadeModel(const char *filename, const char *function, int line, GLenum mode)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glShadeModel(mode);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glShadeModel: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glStencilFunc(const char *filename, const char *function, int line, GLenum func, GLint ref, GLuint mask)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glStencilFunc(func,ref,mask);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glStencilFunc: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glStencilMask(const char *filename, const char *function, int line, GLuint mask)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glStencilMask(mask);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glStencilMask: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glStencilOp(const char *filename, const char *function, int line, GLenum fail, GLenum zfail, GLenum zpass)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glStencilOp(fail,zfail,zpass);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glStencilOp: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexEnvf(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexEnvf(target,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexEnvf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexEnvfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexEnvfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexEnvfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexEnvi(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexEnvi(target,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexEnvi: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexEnviv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexEnviv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexEnviv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexGend(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLdouble param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexGend(coord,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexGend: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexGendv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, const GLdouble *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexGendv(coord,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexGendv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexGenf(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexGenf(coord,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexGenf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexGenfv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, const GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexGenfv(coord,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexGenfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexGeni(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexGeni(coord,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexGeni: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexGeniv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, const GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexGeniv(coord,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexGeniv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexImage1D(target,level,internalFormat,width,border,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexImage1D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexImage2D(target,level,internalFormat,width,height,border,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexImage2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexImage3D(target,level,internalFormat,width,height,depth,border,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexImage3D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexParameterf(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexParameterf(target,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexParameterf: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLfloat *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexParameterfv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexParameterfv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexParameteri(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint param)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexParameteri(target,pname,param);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexParameteri: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLint *params)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexParameteriv(target,pname,params);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexParameteriv: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexSubImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexSubImage1D(target,level,xoffset,width,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexSubImage1D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexSubImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexSubImage2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTexSubImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTexSubImage3D(target,level,xoffset,yoffset,zoffset,width,height,depth,format,type,pixels);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTexSubImage3D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTranslated(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTranslated(x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTranslated: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glTranslatef(const char *filename, const char *function, int line, GLfloat x, GLfloat y, GLfloat z)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glTranslatef(x,y,z);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glTranslatef: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_glViewport(const char *filename, const char *function, int line, GLint x, GLint y, GLsizei width, GLsizei height)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    glViewport(x,y,width,height);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL glViewport: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluBeginCurve(const char *filename, const char *function, int line, GLUnurbs* nurb)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluBeginCurve(nurb);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBeginCurve: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluBeginPolygon(const char *filename, const char *function, int line, GLUtesselator* tess)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluBeginPolygon(tess);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBeginPolygon: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluBeginSurface(const char *filename, const char *function, int line, GLUnurbs* nurb)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluBeginSurface(nurb);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBeginSurface: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluBeginTrim(const char *filename, const char *function, int line, GLUnurbs* nurb)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluBeginTrim(nurb);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBeginTrim: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLint
OpenGL_gluBuild1DMipmapLevels(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluBuild1DMipmapLevels(target,internalFormat,width,format,type,level,base,max,data);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBuild1DMipmapLevels: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLint
OpenGL_gluBuild1DMipmaps(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluBuild1DMipmaps(target,internalFormat,width,format,type,data);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBuild1DMipmaps: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLint
OpenGL_gluBuild2DMipmapLevels(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluBuild2DMipmapLevels(target,internalFormat,width,height,format,type,level,base,max,data);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBuild2DMipmapLevels: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLint
OpenGL_gluBuild2DMipmaps(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluBuild2DMipmaps(target,internalFormat,width,height,format,type,data);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBuild2DMipmaps: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLint
OpenGL_gluBuild3DMipmapLevels(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluBuild3DMipmapLevels(target,internalFormat,width,height,depth,format,type,level,base,max,data);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBuild3DMipmapLevels: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLint
OpenGL_gluBuild3DMipmaps(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluBuild3DMipmaps(target,internalFormat,width,height,depth,format,type,data);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluBuild3DMipmaps: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLboolean
OpenGL_gluCheckExtension(const char *filename, const char *function, int line, const GLubyte *extName, const GLubyte *extString)
{
    GLenum glerror;
    GLboolean retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluCheckExtension(extName,extString);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluCheckExtension: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_gluCylinder(const char *filename, const char *function, int line, GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluCylinder(quad,base,top,height,slices,stacks);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluCylinder: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluDeleteNurbsRenderer(const char *filename, const char *function, int line, GLUnurbs* nurb)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluDeleteNurbsRenderer(nurb);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluDeleteNurbsRenderer: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluDeleteQuadric(const char *filename, const char *function, int line, GLUquadric* quad)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluDeleteQuadric(quad);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluDeleteQuadric: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluDeleteTess(const char *filename, const char *function, int line, GLUtesselator* tess)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluDeleteTess(tess);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluDeleteTess: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluDisk(const char *filename, const char *function, int line, GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluDisk(quad,inner,outer,slices,loops);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluDisk: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluEndCurve(const char *filename, const char *function, int line, GLUnurbs* nurb)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluEndCurve(nurb);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluEndCurve: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluEndPolygon(const char *filename, const char *function, int line, GLUtesselator* tess)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluEndPolygon(tess);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluEndPolygon: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluEndSurface(const char *filename, const char *function, int line, GLUnurbs* nurb)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluEndSurface(nurb);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluEndSurface: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluEndTrim(const char *filename, const char *function, int line, GLUnurbs* nurb)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluEndTrim(nurb);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluEndTrim: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

const GLubyte *
OpenGL_gluErrorString(const char *filename, const char *function, int line, GLenum error)
{
    GLenum glerror;
    const GLubyte * retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluErrorString(error);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluErrorString: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_gluGetNurbsProperty(const char *filename, const char *function, int line, GLUnurbs* nurb, GLenum property, GLfloat* data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluGetNurbsProperty(nurb,property,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluGetNurbsProperty: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

const GLubyte *
OpenGL_gluGetString(const char *filename, const char *function, int line, GLenum name)
{
    GLenum glerror;
    const GLubyte * retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluGetString(name);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluGetString: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_gluGetTessProperty(const char *filename, const char *function, int line, GLUtesselator* tess, GLenum which, GLdouble* data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluGetTessProperty(tess,which,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluGetTessProperty: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluLoadSamplingMatrices(const char *filename, const char *function, int line, GLUnurbs* nurb, const GLfloat *model, const GLfloat *perspective, const GLint *view)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluLoadSamplingMatrices(nurb,model,perspective,view);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluLoadSamplingMatrices: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluLookAt(const char *filename, const char *function, int line, GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluLookAt: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLUnurbs*
OpenGL_gluNewNurbsRenderer(const char *filename, const char *function, int line)
{
    GLenum glerror;
    GLUnurbs* retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluNewNurbsRenderer();

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNewNurbsRenderer: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLUquadric*
OpenGL_gluNewQuadric(const char *filename, const char *function, int line)
{
    GLenum glerror;
    GLUquadric* retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluNewQuadric();

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNewQuadric: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLUtesselator*
OpenGL_gluNewTess(const char *filename, const char *function, int line)
{
    GLenum glerror;
    GLUtesselator* retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluNewTess();

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNewTess: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_gluNextContour(const char *filename, const char *function, int line, GLUtesselator* tess, GLenum type)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluNextContour(tess,type);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNextContour: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluNurbsCallback(const char *filename, const char *function, int line, GLUnurbs* nurb, GLenum which, _GLUfuncptr CallBackFunc)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluNurbsCallback(nurb,which,CallBackFunc);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNurbsCallback: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluNurbsCallbackData(const char *filename, const char *function, int line, GLUnurbs* nurb, GLvoid* userData)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluNurbsCallbackData(nurb,userData);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNurbsCallbackData: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluNurbsCallbackDataEXT(const char *filename, const char *function, int line, GLUnurbs* nurb, GLvoid* userData)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluNurbsCallbackDataEXT(nurb,userData);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNurbsCallbackDataEXT: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluNurbsCurve(const char *filename, const char *function, int line, GLUnurbs* nurb, GLint knotCount, GLfloat *knots, GLint stride, GLfloat *control, GLint order, GLenum type)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluNurbsCurve(nurb,knotCount,knots,stride,control,order,type);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNurbsCurve: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluNurbsProperty(const char *filename, const char *function, int line, GLUnurbs* nurb, GLenum property, GLfloat value)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluNurbsProperty(nurb,property,value);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNurbsProperty: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluNurbsSurface(const char *filename, const char *function, int line, GLUnurbs* nurb, GLint sKnotCount, GLfloat* sKnots, GLint tKnotCount, GLfloat* tKnots, GLint sStride, GLint tStride, GLfloat* control, GLint sOrder, GLint tOrder, GLenum type)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluNurbsSurface(nurb,sKnotCount,sKnots,tKnotCount,tKnots,sStride,tStride,control,sOrder,tOrder,type);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluNurbsSurface: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluOrtho2D(const char *filename, const char *function, int line, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluOrtho2D(left,right,bottom,top);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluOrtho2D: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluPartialDisk(const char *filename, const char *function, int line, GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops, GLdouble start, GLdouble sweep)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluPartialDisk(quad,inner,outer,slices,loops,start,sweep);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluPartialDisk: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluPerspective(const char *filename, const char *function, int line, GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluPerspective(fovy,aspect,zNear,zFar);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluPerspective: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluPickMatrix(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluPickMatrix(x,y,delX,delY,viewport);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluPickMatrix: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLint
OpenGL_gluProject(const char *filename, const char *function, int line, GLdouble objX, GLdouble objY, GLdouble objZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* winX, GLdouble* winY, GLdouble* winZ)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluProject(objX,objY,objZ,model,proj,view,winX,winY,winZ);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluProject: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_gluPwlCurve(const char *filename, const char *function, int line, GLUnurbs* nurb, GLint count, GLfloat* data, GLint stride, GLenum type)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluPwlCurve(nurb,count,data,stride,type);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluPwlCurve: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluQuadricCallback(const char *filename, const char *function, int line, GLUquadric* quad, GLenum which, _GLUfuncptr CallBackFunc)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluQuadricCallback(quad,which,CallBackFunc);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluQuadricCallback: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluQuadricDrawStyle(const char *filename, const char *function, int line, GLUquadric* quad, GLenum draw)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluQuadricDrawStyle(quad,draw);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluQuadricDrawStyle: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluQuadricNormals(const char *filename, const char *function, int line, GLUquadric* quad, GLenum normal)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluQuadricNormals(quad,normal);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluQuadricNormals: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluQuadricOrientation(const char *filename, const char *function, int line, GLUquadric* quad, GLenum orientation)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluQuadricOrientation(quad,orientation);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluQuadricOrientation: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluQuadricTexture(const char *filename, const char *function, int line, GLUquadric* quad, GLboolean texture)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluQuadricTexture(quad,texture);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluQuadricTexture: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLint
OpenGL_gluScaleImage(const char *filename, const char *function, int line, GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluScaleImage(format,wIn,hIn,typeIn,dataIn,wOut,hOut,typeOut,dataOut);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluScaleImage: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

void
OpenGL_gluSphere(const char *filename, const char *function, int line, GLUquadric* quad, GLdouble radius, GLint slices, GLint stacks)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluSphere(quad,radius,slices,stacks);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluSphere: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluTessBeginContour(const char *filename, const char *function, int line, GLUtesselator* tess)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluTessBeginContour(tess);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluTessBeginContour: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluTessBeginPolygon(const char *filename, const char *function, int line, GLUtesselator* tess, GLvoid* data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluTessBeginPolygon(tess,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluTessBeginPolygon: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluTessCallback(const char *filename, const char *function, int line, GLUtesselator* tess, GLenum which, _GLUfuncptr CallBackFunc)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluTessCallback(tess,which,CallBackFunc);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluTessCallback: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluTessEndContour(const char *filename, const char *function, int line, GLUtesselator* tess)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluTessEndContour(tess);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluTessEndContour: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluTessEndPolygon(const char *filename, const char *function, int line, GLUtesselator* tess)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluTessEndPolygon(tess);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluTessEndPolygon: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluTessNormal(const char *filename, const char *function, int line, GLUtesselator* tess, GLdouble valueX, GLdouble valueY, GLdouble valueZ)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluTessNormal(tess,valueX,valueY,valueZ);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluTessNormal: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluTessProperty(const char *filename, const char *function, int line, GLUtesselator* tess, GLenum which, GLdouble data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluTessProperty(tess,which,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluTessProperty: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

void
OpenGL_gluTessVertex(const char *filename, const char *function, int line, GLUtesselator* tess, GLdouble *location, GLvoid* data)
{
    GLenum glerror;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    gluTessVertex(tess,location,data);

    glerror = glGetError();
    if (glerror == 0)
        return;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluTessVertex: %s",
            filename, function, line, gl_strerror(glerror)));

    return;
}

GLint
OpenGL_gluUnProject(const char *filename, const char *function, int line, GLdouble winX, GLdouble winY, GLdouble winZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* objX, GLdouble* objY, GLdouble* objZ)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluUnProject(winX,winY,winZ,model,proj,view,objX,objY,objZ);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluUnProject: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

GLint
OpenGL_gluUnProject4(const char *filename, const char *function, int line, GLdouble winX, GLdouble winY, GLdouble winZ, GLdouble clipW, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble nearVal, GLdouble farVal, GLdouble* objX, GLdouble* objY, GLdouble* objZ, GLdouble* objW)
{
    GLenum glerror;
    GLint retval;

#if (CLEAR_ERRORS > 0)
    gl_clear_error();
#endif
    retval = gluUnProject4(winX,winY,winZ,clipW,model,proj,view,nearVal,farVal,objX,objY,objZ,objW);

    glerror = glGetError();
    if (glerror == 0)
        return retval;

    gl_throw_error(mkstr("[%s::%s:%d]: Error after calling OpenGL gluUnProject4: %s",
            filename, function, line, gl_strerror(glerror)));

    return retval;
}

