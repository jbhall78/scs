#ifndef _SYS_GL_AUTO_H
#define _SYS_GL_AUTO_H
#include <GL/gl.h>
#include <GL/glu.h>

void OpenGL_glAccum(const char *filename, const char *function, int line, GLenum op, GLfloat value);
void OpenGL_glActiveTexture(const char *filename, const char *function, int line, GLenum texture);
void OpenGL_glActiveTextureARB(const char *filename, const char *function, int line, GLenum texture);
void OpenGL_glAlphaFunc(const char *filename, const char *function, int line, GLenum func, GLclampf ref);
GLboolean OpenGL_glAreTexturesResident(const char *filename, const char *function, int line, GLsizei n, const GLuint *textures, GLboolean *residences);
void OpenGL_glArrayElement(const char *filename, const char *function, int line, GLint i);
void OpenGL_glBindTexture(const char *filename, const char *function, int line, GLenum target, GLuint texture);
void OpenGL_glBitmap(const char *filename, const char *function, int line, GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
void OpenGL_glBlendColor(const char *filename, const char *function, int line, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void OpenGL_glBlendEquation(const char *filename, const char *function, int line, GLenum mode);
void OpenGL_glBlendFunc(const char *filename, const char *function, int line, GLenum sfactor, GLenum dfactor);
void OpenGL_glCallList(const char *filename, const char *function, int line, GLuint list);
void OpenGL_glCallLists(const char *filename, const char *function, int line, GLsizei n, GLenum type, const GLvoid *lists);
void OpenGL_glClear(const char *filename, const char *function, int line, GLbitfield mask);
void OpenGL_glClearAccum(const char *filename, const char *function, int line, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void OpenGL_glClearColor(const char *filename, const char *function, int line, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void OpenGL_glClearDepth(const char *filename, const char *function, int line, GLclampd depth);
void OpenGL_glClearIndex(const char *filename, const char *function, int line, GLfloat c);
void OpenGL_glClearStencil(const char *filename, const char *function, int line, GLint s);
void OpenGL_glClientActiveTexture(const char *filename, const char *function, int line, GLenum texture);
void OpenGL_glClientActiveTextureARB(const char *filename, const char *function, int line, GLenum texture);
void OpenGL_glClipPlane(const char *filename, const char *function, int line, GLenum plane, const GLdouble *equation);
void OpenGL_glCompressedTexImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
void OpenGL_glCompressedTexImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
void OpenGL_glCompressedTexImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
void OpenGL_glCompressedTexSubImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
void OpenGL_glCompressedTexSubImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
void OpenGL_glCompressedTexSubImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
void OpenGL_glConvolutionFilter1D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
void OpenGL_glConvolutionFilter2D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
void OpenGL_glConvolutionParameterf(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat params);
void OpenGL_glConvolutionParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLfloat *params);
void OpenGL_glConvolutionParameteri(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint params);
void OpenGL_glConvolutionParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLint *params);
void OpenGL_glCopyColorSubTable(const char *filename, const char *function, int line, GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
void OpenGL_glCopyColorTable(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void OpenGL_glCopyConvolutionFilter1D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void OpenGL_glCopyConvolutionFilter2D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
void OpenGL_glCopyPixels(const char *filename, const char *function, int line, GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
void OpenGL_glCopyTexImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void OpenGL_glCopyTexImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void OpenGL_glCopyTexSubImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void OpenGL_glCopyTexSubImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void OpenGL_glCopyTexSubImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void OpenGL_glCullFace(const char *filename, const char *function, int line, GLenum mode);
void OpenGL_glDeleteLists(const char *filename, const char *function, int line, GLuint list, GLsizei range);
void OpenGL_glDeleteTextures(const char *filename, const char *function, int line, GLsizei n, const GLuint *textures);
void OpenGL_glDepthFunc(const char *filename, const char *function, int line, GLenum func);
void OpenGL_glDepthMask(const char *filename, const char *function, int line, GLboolean flag);
void OpenGL_glDepthRange(const char *filename, const char *function, int line, GLclampd near_val, GLclampd far_val);
void OpenGL_glDisable(const char *filename, const char *function, int line, GLenum cap);
void OpenGL_glDisableClientState(const char *filename, const char *function, int line, GLenum cap);
void OpenGL_glDrawArrays(const char *filename, const char *function, int line, GLenum mode, GLint first, GLsizei count);
void OpenGL_glDrawBuffer(const char *filename, const char *function, int line, GLenum mode);
void OpenGL_glDrawElements(const char *filename, const char *function, int line, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void OpenGL_glDrawPixels(const char *filename, const char *function, int line, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void OpenGL_glDrawRangeElements(const char *filename, const char *function, int line, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
void OpenGL_glEdgeFlag(const char *filename, const char *function, int line, GLboolean flag);
void OpenGL_glEdgeFlagPointer(const char *filename, const char *function, int line, GLsizei stride, const GLvoid *ptr);
void OpenGL_glEdgeFlagv(const char *filename, const char *function, int line, const GLboolean *flag);
void OpenGL_glEnable(const char *filename, const char *function, int line, GLenum cap);
void OpenGL_glEnableClientState(const char *filename, const char *function, int line, GLenum cap);
void OpenGL_glEvalCoord1d(const char *filename, const char *function, int line, GLdouble u);
void OpenGL_glEvalCoord1dv(const char *filename, const char *function, int line, const GLdouble *u);
void OpenGL_glEvalCoord1f(const char *filename, const char *function, int line, GLfloat u);
void OpenGL_glEvalCoord1fv(const char *filename, const char *function, int line, const GLfloat *u);
void OpenGL_glEvalCoord2d(const char *filename, const char *function, int line, GLdouble u, GLdouble v);
void OpenGL_glEvalCoord2dv(const char *filename, const char *function, int line, const GLdouble *u);
void OpenGL_glEvalCoord2f(const char *filename, const char *function, int line, GLfloat u, GLfloat v);
void OpenGL_glEvalCoord2fv(const char *filename, const char *function, int line, const GLfloat *u);
void OpenGL_glEvalMesh1(const char *filename, const char *function, int line, GLenum mode, GLint i1, GLint i2);
void OpenGL_glEvalMesh2(const char *filename, const char *function, int line, GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
void OpenGL_glEvalPoint1(const char *filename, const char *function, int line, GLint i);
void OpenGL_glEvalPoint2(const char *filename, const char *function, int line, GLint i, GLint j);
void OpenGL_glFeedbackBuffer(const char *filename, const char *function, int line, GLsizei size, GLenum type, GLfloat *buffer);
void OpenGL_glFinish(const char *filename, const char *function, int line);
void OpenGL_glFlush(const char *filename, const char *function, int line);
void OpenGL_glFogf(const char *filename, const char *function, int line, GLenum pname, GLfloat param);
void OpenGL_glFogfv(const char *filename, const char *function, int line, GLenum pname, const GLfloat *params);
void OpenGL_glFogi(const char *filename, const char *function, int line, GLenum pname, GLint param);
void OpenGL_glFogiv(const char *filename, const char *function, int line, GLenum pname, const GLint *params);
void OpenGL_glFrontFace(const char *filename, const char *function, int line, GLenum mode);
void OpenGL_glFrustum(const char *filename, const char *function, int line, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
GLuint OpenGL_glGenLists(const char *filename, const char *function, int line, GLsizei range);
void OpenGL_glGenTextures(const char *filename, const char *function, int line, GLsizei n, GLuint *textures);
void OpenGL_glGetBooleanv(const char *filename, const char *function, int line, GLenum pname, GLboolean *params);
void OpenGL_glGetClipPlane(const char *filename, const char *function, int line, GLenum plane, GLdouble *equation);
void OpenGL_glGetColorTable(const char *filename, const char *function, int line, GLenum target, GLenum format, GLenum type, GLvoid *table);
void OpenGL_glGetColorTableParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params);
void OpenGL_glGetColorTableParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params);
void OpenGL_glGetCompressedTexImage(const char *filename, const char *function, int line, GLenum target, GLint lod, GLvoid *img);
void OpenGL_glGetConvolutionFilter(const char *filename, const char *function, int line, GLenum target, GLenum format, GLenum type, GLvoid *image);
void OpenGL_glGetConvolutionParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params);
void OpenGL_glGetConvolutionParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params);
void OpenGL_glGetDoublev(const char *filename, const char *function, int line, GLenum pname, GLdouble *params);
GLenum OpenGL_glGetError(const char *filename, const char *function, int line);
void OpenGL_glGetFloatv(const char *filename, const char *function, int line, GLenum pname, GLfloat *params);
void OpenGL_glGetHistogram(const char *filename, const char *function, int line, GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void OpenGL_glGetHistogramParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params);
void OpenGL_glGetHistogramParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params);
void OpenGL_glGetIntegerv(const char *filename, const char *function, int line, GLenum pname, GLint *params);
void OpenGL_glGetLightfv(const char *filename, const char *function, int line, GLenum light, GLenum pname, GLfloat *params);
void OpenGL_glGetLightiv(const char *filename, const char *function, int line, GLenum light, GLenum pname, GLint *params);
void OpenGL_glGetMapdv(const char *filename, const char *function, int line, GLenum target, GLenum query, GLdouble *v);
void OpenGL_glGetMapfv(const char *filename, const char *function, int line, GLenum target, GLenum query, GLfloat *v);
void OpenGL_glGetMapiv(const char *filename, const char *function, int line, GLenum target, GLenum query, GLint *v);
void OpenGL_glGetMaterialfv(const char *filename, const char *function, int line, GLenum face, GLenum pname, GLfloat *params);
void OpenGL_glGetMaterialiv(const char *filename, const char *function, int line, GLenum face, GLenum pname, GLint *params);
void OpenGL_glGetMinmax(const char *filename, const char *function, int line, GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values);
void OpenGL_glGetMinmaxParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params);
void OpenGL_glGetMinmaxParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params);
void OpenGL_glGetPixelMapfv(const char *filename, const char *function, int line, GLenum map, GLfloat *values);
void OpenGL_glGetPixelMapuiv(const char *filename, const char *function, int line, GLenum map, GLuint *values);
void OpenGL_glGetPixelMapusv(const char *filename, const char *function, int line, GLenum map, GLushort *values);
void OpenGL_glGetPointerv(const char *filename, const char *function, int line, GLenum pname, GLvoid **params);
void OpenGL_glGetPolygonStipple(const char *filename, const char *function, int line, GLubyte *mask);
void OpenGL_glGetSeparableFilter(const char *filename, const char *function, int line, GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
const GLubyte * OpenGL_glGetString(const char *filename, const char *function, int line, GLenum name);
void OpenGL_glGetTexEnvfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params);
void OpenGL_glGetTexEnviv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params);
void OpenGL_glGetTexGendv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLdouble *params);
void OpenGL_glGetTexGenfv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLfloat *params);
void OpenGL_glGetTexGeniv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLint *params);
void OpenGL_glGetTexImage(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void OpenGL_glGetTexLevelParameterfv(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum pname, GLfloat *params);
void OpenGL_glGetTexLevelParameteriv(const char *filename, const char *function, int line, GLenum target, GLint level, GLenum pname, GLint *params);
void OpenGL_glGetTexParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat *params);
void OpenGL_glGetTexParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint *params);
void OpenGL_glHint(const char *filename, const char *function, int line, GLenum target, GLenum mode);
void OpenGL_glHistogram(const char *filename, const char *function, int line, GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
void OpenGL_glIndexMask(const char *filename, const char *function, int line, GLuint mask);
void OpenGL_glIndexPointer(const char *filename, const char *function, int line, GLenum type, GLsizei stride, const GLvoid *ptr);
void OpenGL_glIndexd(const char *filename, const char *function, int line, GLdouble c);
void OpenGL_glIndexdv(const char *filename, const char *function, int line, const GLdouble *c);
void OpenGL_glIndexf(const char *filename, const char *function, int line, GLfloat c);
void OpenGL_glIndexfv(const char *filename, const char *function, int line, const GLfloat *c);
void OpenGL_glIndexi(const char *filename, const char *function, int line, GLint c);
void OpenGL_glIndexiv(const char *filename, const char *function, int line, const GLint *c);
void OpenGL_glIndexs(const char *filename, const char *function, int line, GLshort c);
void OpenGL_glIndexsv(const char *filename, const char *function, int line, const GLshort *c);
void OpenGL_glIndexub(const char *filename, const char *function, int line, GLubyte c);
void OpenGL_glIndexubv(const char *filename, const char *function, int line, const GLubyte *c);
void OpenGL_glInitNames(const char *filename, const char *function, int line);
void OpenGL_glInterleavedArrays(const char *filename, const char *function, int line, GLenum format, GLsizei stride, const GLvoid *pointer);
GLboolean OpenGL_glIsEnabled(const char *filename, const char *function, int line, GLenum cap);
GLboolean OpenGL_glIsList(const char *filename, const char *function, int line, GLuint list);
GLboolean OpenGL_glIsTexture(const char *filename, const char *function, int line, GLuint texture);
void OpenGL_glLightModelf(const char *filename, const char *function, int line, GLenum pname, GLfloat param);
void OpenGL_glLightModelfv(const char *filename, const char *function, int line, GLenum pname, const GLfloat *params);
void OpenGL_glLightModeli(const char *filename, const char *function, int line, GLenum pname, GLint param);
void OpenGL_glLightModeliv(const char *filename, const char *function, int line, GLenum pname, const GLint *params);
void OpenGL_glLightf(const char *filename, const char *function, int line, GLenum light, GLenum pname, GLfloat param);
void OpenGL_glLightfv(const char *filename, const char *function, int line, GLenum light, GLenum pname, const GLfloat *params);
void OpenGL_glLighti(const char *filename, const char *function, int line, GLenum light, GLenum pname, GLint param);
void OpenGL_glLightiv(const char *filename, const char *function, int line, GLenum light, GLenum pname, const GLint *params);
void OpenGL_glLineStipple(const char *filename, const char *function, int line, GLint factor, GLushort pattern);
void OpenGL_glLineWidth(const char *filename, const char *function, int line, GLfloat width);
void OpenGL_glListBase(const char *filename, const char *function, int line, GLuint base);
void OpenGL_glLoadIdentity(const char *filename, const char *function, int line);
void OpenGL_glLoadMatrixd(const char *filename, const char *function, int line, const GLdouble *m);
void OpenGL_glLoadMatrixf(const char *filename, const char *function, int line, const GLfloat *m);
void OpenGL_glLoadName(const char *filename, const char *function, int line, GLuint name);
void OpenGL_glLoadTransposeMatrixd(const char *filename, const char *function, int line, const GLdouble m[16]);
void OpenGL_glLoadTransposeMatrixf(const char *filename, const char *function, int line, const GLfloat m[16]);
void OpenGL_glLogicOp(const char *filename, const char *function, int line, GLenum opcode);
void OpenGL_glMap1d(const char *filename, const char *function, int line, GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
void OpenGL_glMap1f(const char *filename, const char *function, int line, GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
void OpenGL_glMap2d(const char *filename, const char *function, int line, GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
void OpenGL_glMap2f(const char *filename, const char *function, int line, GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
void OpenGL_glMapGrid1d(const char *filename, const char *function, int line, GLint un, GLdouble u1, GLdouble u2);
void OpenGL_glMapGrid1f(const char *filename, const char *function, int line, GLint un, GLfloat u1, GLfloat u2);
void OpenGL_glMapGrid2d(const char *filename, const char *function, int line, GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
void OpenGL_glMapGrid2f(const char *filename, const char *function, int line, GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
void OpenGL_glMaterialf(const char *filename, const char *function, int line, GLenum face, GLenum pname, GLfloat param);
void OpenGL_glMaterialfv(const char *filename, const char *function, int line, GLenum face, GLenum pname, const GLfloat *params);
void OpenGL_glMateriali(const char *filename, const char *function, int line, GLenum face, GLenum pname, GLint param);
void OpenGL_glMaterialiv(const char *filename, const char *function, int line, GLenum face, GLenum pname, const GLint *params);
void OpenGL_glMatrixMode(const char *filename, const char *function, int line, GLenum mode);
void OpenGL_glMinmax(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLboolean sink);
void OpenGL_glMultMatrixd(const char *filename, const char *function, int line, const GLdouble *m);
void OpenGL_glMultMatrixf(const char *filename, const char *function, int line, const GLfloat *m);
void OpenGL_glMultTransposeMatrixd(const char *filename, const char *function, int line, const GLdouble m[16]);
void OpenGL_glMultTransposeMatrixf(const char *filename, const char *function, int line, const GLfloat m[16]);
void OpenGL_glMultiTexCoord1d(const char *filename, const char *function, int line, GLenum target, GLdouble s);
void OpenGL_glMultiTexCoord1dARB(const char *filename, const char *function, int line, GLenum target, GLdouble s);
void OpenGL_glMultiTexCoord1dv(const char *filename, const char *function, int line, GLenum target, const GLdouble *v);
void OpenGL_glMultiTexCoord1dvARB(const char *filename, const char *function, int line, GLenum target, const GLdouble *v);
void OpenGL_glMultiTexCoord1f(const char *filename, const char *function, int line, GLenum target, GLfloat s);
void OpenGL_glMultiTexCoord1fARB(const char *filename, const char *function, int line, GLenum target, GLfloat s);
void OpenGL_glMultiTexCoord1fv(const char *filename, const char *function, int line, GLenum target, const GLfloat *v);
void OpenGL_glMultiTexCoord1fvARB(const char *filename, const char *function, int line, GLenum target, const GLfloat *v);
void OpenGL_glMultiTexCoord1i(const char *filename, const char *function, int line, GLenum target, GLint s);
void OpenGL_glMultiTexCoord1iARB(const char *filename, const char *function, int line, GLenum target, GLint s);
void OpenGL_glMultiTexCoord1iv(const char *filename, const char *function, int line, GLenum target, const GLint *v);
void OpenGL_glMultiTexCoord1ivARB(const char *filename, const char *function, int line, GLenum target, const GLint *v);
void OpenGL_glMultiTexCoord1s(const char *filename, const char *function, int line, GLenum target, GLshort s);
void OpenGL_glMultiTexCoord1sARB(const char *filename, const char *function, int line, GLenum target, GLshort s);
void OpenGL_glMultiTexCoord1sv(const char *filename, const char *function, int line, GLenum target, const GLshort *v);
void OpenGL_glMultiTexCoord1svARB(const char *filename, const char *function, int line, GLenum target, const GLshort *v);
void OpenGL_glMultiTexCoord2d(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t);
void OpenGL_glMultiTexCoord2dARB(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t);
void OpenGL_glMultiTexCoord2dv(const char *filename, const char *function, int line, GLenum target, const GLdouble *v);
void OpenGL_glMultiTexCoord2dvARB(const char *filename, const char *function, int line, GLenum target, const GLdouble *v);
void OpenGL_glMultiTexCoord2f(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t);
void OpenGL_glMultiTexCoord2fARB(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t);
void OpenGL_glMultiTexCoord2fv(const char *filename, const char *function, int line, GLenum target, const GLfloat *v);
void OpenGL_glMultiTexCoord2fvARB(const char *filename, const char *function, int line, GLenum target, const GLfloat *v);
void OpenGL_glMultiTexCoord2i(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t);
void OpenGL_glMultiTexCoord2iARB(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t);
void OpenGL_glMultiTexCoord2iv(const char *filename, const char *function, int line, GLenum target, const GLint *v);
void OpenGL_glMultiTexCoord2ivARB(const char *filename, const char *function, int line, GLenum target, const GLint *v);
void OpenGL_glMultiTexCoord2s(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t);
void OpenGL_glMultiTexCoord2sARB(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t);
void OpenGL_glMultiTexCoord2sv(const char *filename, const char *function, int line, GLenum target, const GLshort *v);
void OpenGL_glMultiTexCoord2svARB(const char *filename, const char *function, int line, GLenum target, const GLshort *v);
void OpenGL_glMultiTexCoord3d(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t, GLdouble r);
void OpenGL_glMultiTexCoord3dARB(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t, GLdouble r);
void OpenGL_glMultiTexCoord3dv(const char *filename, const char *function, int line, GLenum target, const GLdouble *v);
void OpenGL_glMultiTexCoord3dvARB(const char *filename, const char *function, int line, GLenum target, const GLdouble *v);
void OpenGL_glMultiTexCoord3f(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t, GLfloat r);
void OpenGL_glMultiTexCoord3fARB(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t, GLfloat r);
void OpenGL_glMultiTexCoord3fv(const char *filename, const char *function, int line, GLenum target, const GLfloat *v);
void OpenGL_glMultiTexCoord3fvARB(const char *filename, const char *function, int line, GLenum target, const GLfloat *v);
void OpenGL_glMultiTexCoord3i(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t, GLint r);
void OpenGL_glMultiTexCoord3iARB(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t, GLint r);
void OpenGL_glMultiTexCoord3iv(const char *filename, const char *function, int line, GLenum target, const GLint *v);
void OpenGL_glMultiTexCoord3ivARB(const char *filename, const char *function, int line, GLenum target, const GLint *v);
void OpenGL_glMultiTexCoord3s(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t, GLshort r);
void OpenGL_glMultiTexCoord3sARB(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t, GLshort r);
void OpenGL_glMultiTexCoord3sv(const char *filename, const char *function, int line, GLenum target, const GLshort *v);
void OpenGL_glMultiTexCoord3svARB(const char *filename, const char *function, int line, GLenum target, const GLshort *v);
void OpenGL_glMultiTexCoord4d(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void OpenGL_glMultiTexCoord4dARB(const char *filename, const char *function, int line, GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void OpenGL_glMultiTexCoord4dv(const char *filename, const char *function, int line, GLenum target, const GLdouble *v);
void OpenGL_glMultiTexCoord4dvARB(const char *filename, const char *function, int line, GLenum target, const GLdouble *v);
void OpenGL_glMultiTexCoord4f(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void OpenGL_glMultiTexCoord4fARB(const char *filename, const char *function, int line, GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void OpenGL_glMultiTexCoord4fv(const char *filename, const char *function, int line, GLenum target, const GLfloat *v);
void OpenGL_glMultiTexCoord4fvARB(const char *filename, const char *function, int line, GLenum target, const GLfloat *v);
void OpenGL_glMultiTexCoord4i(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t, GLint r, GLint q);
void OpenGL_glMultiTexCoord4iARB(const char *filename, const char *function, int line, GLenum target, GLint s, GLint t, GLint r, GLint q);
void OpenGL_glMultiTexCoord4iv(const char *filename, const char *function, int line, GLenum target, const GLint *v);
void OpenGL_glMultiTexCoord4ivARB(const char *filename, const char *function, int line, GLenum target, const GLint *v);
void OpenGL_glMultiTexCoord4s(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
void OpenGL_glMultiTexCoord4sARB(const char *filename, const char *function, int line, GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
void OpenGL_glMultiTexCoord4sv(const char *filename, const char *function, int line, GLenum target, const GLshort *v);
void OpenGL_glMultiTexCoord4svARB(const char *filename, const char *function, int line, GLenum target, const GLshort *v);
void OpenGL_glNewList(const char *filename, const char *function, int line, GLuint list, GLenum mode);
void OpenGL_glOrtho(const char *filename, const char *function, int line, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
void OpenGL_glPassThrough(const char *filename, const char *function, int line, GLfloat token);
void OpenGL_glPixelMapfv(const char *filename, const char *function, int line, GLenum map, GLsizei mapsize, const GLfloat *values);
void OpenGL_glPixelMapuiv(const char *filename, const char *function, int line, GLenum map, GLsizei mapsize, const GLuint *values);
void OpenGL_glPixelMapusv(const char *filename, const char *function, int line, GLenum map, GLsizei mapsize, const GLushort *values);
void OpenGL_glPixelStoref(const char *filename, const char *function, int line, GLenum pname, GLfloat param);
void OpenGL_glPixelStorei(const char *filename, const char *function, int line, GLenum pname, GLint param);
void OpenGL_glPixelTransferf(const char *filename, const char *function, int line, GLenum pname, GLfloat param);
void OpenGL_glPixelTransferi(const char *filename, const char *function, int line, GLenum pname, GLint param);
void OpenGL_glPixelZoom(const char *filename, const char *function, int line, GLfloat xfactor, GLfloat yfactor);
void OpenGL_glPointSize(const char *filename, const char *function, int line, GLfloat size);
void OpenGL_glPolygonMode(const char *filename, const char *function, int line, GLenum face, GLenum mode);
void OpenGL_glPolygonOffset(const char *filename, const char *function, int line, GLfloat factor, GLfloat units);
void OpenGL_glPolygonStipple(const char *filename, const char *function, int line, const GLubyte *mask);
void OpenGL_glPopAttrib(const char *filename, const char *function, int line);
void OpenGL_glPopClientAttrib(const char *filename, const char *function, int line);
void OpenGL_glPopMatrix(const char *filename, const char *function, int line);
void OpenGL_glPopName(const char *filename, const char *function, int line);
void OpenGL_glPrioritizeTextures(const char *filename, const char *function, int line, GLsizei n, const GLuint *textures, const GLclampf *priorities);
void OpenGL_glPushAttrib(const char *filename, const char *function, int line, GLbitfield mask);
void OpenGL_glPushClientAttrib(const char *filename, const char *function, int line, GLbitfield mask);
void OpenGL_glPushMatrix(const char *filename, const char *function, int line);
void OpenGL_glPushName(const char *filename, const char *function, int line, GLuint name);
void OpenGL_glRasterPos2d(const char *filename, const char *function, int line, GLdouble x, GLdouble y);
void OpenGL_glRasterPos2dv(const char *filename, const char *function, int line, const GLdouble *v);
void OpenGL_glRasterPos2f(const char *filename, const char *function, int line, GLfloat x, GLfloat y);
void OpenGL_glRasterPos2fv(const char *filename, const char *function, int line, const GLfloat *v);
void OpenGL_glRasterPos2i(const char *filename, const char *function, int line, GLint x, GLint y);
void OpenGL_glRasterPos2iv(const char *filename, const char *function, int line, const GLint *v);
void OpenGL_glRasterPos2s(const char *filename, const char *function, int line, GLshort x, GLshort y);
void OpenGL_glRasterPos2sv(const char *filename, const char *function, int line, const GLshort *v);
void OpenGL_glRasterPos3d(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble z);
void OpenGL_glRasterPos3dv(const char *filename, const char *function, int line, const GLdouble *v);
void OpenGL_glRasterPos3f(const char *filename, const char *function, int line, GLfloat x, GLfloat y, GLfloat z);
void OpenGL_glRasterPos3fv(const char *filename, const char *function, int line, const GLfloat *v);
void OpenGL_glRasterPos3i(const char *filename, const char *function, int line, GLint x, GLint y, GLint z);
void OpenGL_glRasterPos3iv(const char *filename, const char *function, int line, const GLint *v);
void OpenGL_glRasterPos3s(const char *filename, const char *function, int line, GLshort x, GLshort y, GLshort z);
void OpenGL_glRasterPos3sv(const char *filename, const char *function, int line, const GLshort *v);
void OpenGL_glRasterPos4d(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void OpenGL_glRasterPos4dv(const char *filename, const char *function, int line, const GLdouble *v);
void OpenGL_glRasterPos4f(const char *filename, const char *function, int line, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void OpenGL_glRasterPos4fv(const char *filename, const char *function, int line, const GLfloat *v);
void OpenGL_glRasterPos4i(const char *filename, const char *function, int line, GLint x, GLint y, GLint z, GLint w);
void OpenGL_glRasterPos4iv(const char *filename, const char *function, int line, const GLint *v);
void OpenGL_glRasterPos4s(const char *filename, const char *function, int line, GLshort x, GLshort y, GLshort z, GLshort w);
void OpenGL_glRasterPos4sv(const char *filename, const char *function, int line, const GLshort *v);
void OpenGL_glReadBuffer(const char *filename, const char *function, int line, GLenum mode);
void OpenGL_glReadPixels(const char *filename, const char *function, int line, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void OpenGL_glRectd(const char *filename, const char *function, int line, GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void OpenGL_glRectdv(const char *filename, const char *function, int line, const GLdouble *v1, const GLdouble *v2);
void OpenGL_glRectf(const char *filename, const char *function, int line, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void OpenGL_glRectfv(const char *filename, const char *function, int line, const GLfloat *v1, const GLfloat *v2);
void OpenGL_glRecti(const char *filename, const char *function, int line, GLint x1, GLint y1, GLint x2, GLint y2);
void OpenGL_glRectiv(const char *filename, const char *function, int line, const GLint *v1, const GLint *v2);
void OpenGL_glRects(const char *filename, const char *function, int line, GLshort x1, GLshort y1, GLshort x2, GLshort y2);
void OpenGL_glRectsv(const char *filename, const char *function, int line, const GLshort *v1, const GLshort *v2);
GLint OpenGL_glRenderMode(const char *filename, const char *function, int line, GLenum mode);
void OpenGL_glResetHistogram(const char *filename, const char *function, int line, GLenum target);
void OpenGL_glResetMinmax(const char *filename, const char *function, int line, GLenum target);
void OpenGL_glRotated(const char *filename, const char *function, int line, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void OpenGL_glRotatef(const char *filename, const char *function, int line, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void OpenGL_glSampleCoverage(const char *filename, const char *function, int line, GLclampf value, GLboolean invert);
void OpenGL_glScaled(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble z);
void OpenGL_glScalef(const char *filename, const char *function, int line, GLfloat x, GLfloat y, GLfloat z);
void OpenGL_glScissor(const char *filename, const char *function, int line, GLint x, GLint y, GLsizei width, GLsizei height);
void OpenGL_glSelectBuffer(const char *filename, const char *function, int line, GLsizei size, GLuint *buffer);
void OpenGL_glSeparableFilter2D(const char *filename, const char *function, int line, GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
void OpenGL_glShadeModel(const char *filename, const char *function, int line, GLenum mode);
void OpenGL_glStencilFunc(const char *filename, const char *function, int line, GLenum func, GLint ref, GLuint mask);
void OpenGL_glStencilMask(const char *filename, const char *function, int line, GLuint mask);
void OpenGL_glStencilOp(const char *filename, const char *function, int line, GLenum fail, GLenum zfail, GLenum zpass);
void OpenGL_glTexEnvf(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat param);
void OpenGL_glTexEnvfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLfloat *params);
void OpenGL_glTexEnvi(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint param);
void OpenGL_glTexEnviv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLint *params);
void OpenGL_glTexGend(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLdouble param);
void OpenGL_glTexGendv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, const GLdouble *params);
void OpenGL_glTexGenf(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLfloat param);
void OpenGL_glTexGenfv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, const GLfloat *params);
void OpenGL_glTexGeni(const char *filename, const char *function, int line, GLenum coord, GLenum pname, GLint param);
void OpenGL_glTexGeniv(const char *filename, const char *function, int line, GLenum coord, GLenum pname, const GLint *params);
void OpenGL_glTexImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void OpenGL_glTexImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void OpenGL_glTexImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void OpenGL_glTexParameterf(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLfloat param);
void OpenGL_glTexParameterfv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLfloat *params);
void OpenGL_glTexParameteri(const char *filename, const char *function, int line, GLenum target, GLenum pname, GLint param);
void OpenGL_glTexParameteriv(const char *filename, const char *function, int line, GLenum target, GLenum pname, const GLint *params);
void OpenGL_glTexSubImage1D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void OpenGL_glTexSubImage2D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void OpenGL_glTexSubImage3D(const char *filename, const char *function, int line, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
void OpenGL_glTranslated(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble z);
void OpenGL_glTranslatef(const char *filename, const char *function, int line, GLfloat x, GLfloat y, GLfloat z);
void OpenGL_glViewport(const char *filename, const char *function, int line, GLint x, GLint y, GLsizei width, GLsizei height);
void OpenGL_gluBeginCurve(const char *filename, const char *function, int line, GLUnurbs* nurb);
void OpenGL_gluBeginPolygon(const char *filename, const char *function, int line, GLUtesselator* tess);
void OpenGL_gluBeginSurface(const char *filename, const char *function, int line, GLUnurbs* nurb);
void OpenGL_gluBeginTrim(const char *filename, const char *function, int line, GLUnurbs* nurb);
GLint OpenGL_gluBuild1DMipmapLevels(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
GLint OpenGL_gluBuild1DMipmaps(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data);
GLint OpenGL_gluBuild2DMipmapLevels(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
GLint OpenGL_gluBuild2DMipmaps(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data);
GLint OpenGL_gluBuild3DMipmapLevels(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
GLint OpenGL_gluBuild3DMipmaps(const char *filename, const char *function, int line, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
GLboolean OpenGL_gluCheckExtension(const char *filename, const char *function, int line, const GLubyte *extName, const GLubyte *extString);
void OpenGL_gluCylinder(const char *filename, const char *function, int line, GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks);
void OpenGL_gluDeleteNurbsRenderer(const char *filename, const char *function, int line, GLUnurbs* nurb);
void OpenGL_gluDeleteQuadric(const char *filename, const char *function, int line, GLUquadric* quad);
void OpenGL_gluDeleteTess(const char *filename, const char *function, int line, GLUtesselator* tess);
void OpenGL_gluDisk(const char *filename, const char *function, int line, GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops);
void OpenGL_gluEndCurve(const char *filename, const char *function, int line, GLUnurbs* nurb);
void OpenGL_gluEndPolygon(const char *filename, const char *function, int line, GLUtesselator* tess);
void OpenGL_gluEndSurface(const char *filename, const char *function, int line, GLUnurbs* nurb);
void OpenGL_gluEndTrim(const char *filename, const char *function, int line, GLUnurbs* nurb);
const GLubyte * OpenGL_gluErrorString(const char *filename, const char *function, int line, GLenum error);
void OpenGL_gluGetNurbsProperty(const char *filename, const char *function, int line, GLUnurbs* nurb, GLenum property, GLfloat* data);
const GLubyte * OpenGL_gluGetString(const char *filename, const char *function, int line, GLenum name);
void OpenGL_gluGetTessProperty(const char *filename, const char *function, int line, GLUtesselator* tess, GLenum which, GLdouble* data);
void OpenGL_gluLoadSamplingMatrices(const char *filename, const char *function, int line, GLUnurbs* nurb, const GLfloat *model, const GLfloat *perspective, const GLint *view);
void OpenGL_gluLookAt(const char *filename, const char *function, int line, GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
GLUnurbs* OpenGL_gluNewNurbsRenderer(const char *filename, const char *function, int line);
GLUquadric* OpenGL_gluNewQuadric(const char *filename, const char *function, int line);
GLUtesselator* OpenGL_gluNewTess(const char *filename, const char *function, int line);
void OpenGL_gluNextContour(const char *filename, const char *function, int line, GLUtesselator* tess, GLenum type);
void OpenGL_gluNurbsCallback(const char *filename, const char *function, int line, GLUnurbs* nurb, GLenum which, _GLUfuncptr CallBackFunc);
void OpenGL_gluNurbsCallbackData(const char *filename, const char *function, int line, GLUnurbs* nurb, GLvoid* userData);
void OpenGL_gluNurbsCallbackDataEXT(const char *filename, const char *function, int line, GLUnurbs* nurb, GLvoid* userData);
void OpenGL_gluNurbsCurve(const char *filename, const char *function, int line, GLUnurbs* nurb, GLint knotCount, GLfloat *knots, GLint stride, GLfloat *control, GLint order, GLenum type);
void OpenGL_gluNurbsProperty(const char *filename, const char *function, int line, GLUnurbs* nurb, GLenum property, GLfloat value);
void OpenGL_gluNurbsSurface(const char *filename, const char *function, int line, GLUnurbs* nurb, GLint sKnotCount, GLfloat* sKnots, GLint tKnotCount, GLfloat* tKnots, GLint sStride, GLint tStride, GLfloat* control, GLint sOrder, GLint tOrder, GLenum type);
void OpenGL_gluOrtho2D(const char *filename, const char *function, int line, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
void OpenGL_gluPartialDisk(const char *filename, const char *function, int line, GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops, GLdouble start, GLdouble sweep);
void OpenGL_gluPerspective(const char *filename, const char *function, int line, GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
void OpenGL_gluPickMatrix(const char *filename, const char *function, int line, GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport);
GLint OpenGL_gluProject(const char *filename, const char *function, int line, GLdouble objX, GLdouble objY, GLdouble objZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* winX, GLdouble* winY, GLdouble* winZ);
void OpenGL_gluPwlCurve(const char *filename, const char *function, int line, GLUnurbs* nurb, GLint count, GLfloat* data, GLint stride, GLenum type);
void OpenGL_gluQuadricCallback(const char *filename, const char *function, int line, GLUquadric* quad, GLenum which, _GLUfuncptr CallBackFunc);
void OpenGL_gluQuadricDrawStyle(const char *filename, const char *function, int line, GLUquadric* quad, GLenum draw);
void OpenGL_gluQuadricNormals(const char *filename, const char *function, int line, GLUquadric* quad, GLenum normal);
void OpenGL_gluQuadricOrientation(const char *filename, const char *function, int line, GLUquadric* quad, GLenum orientation);
void OpenGL_gluQuadricTexture(const char *filename, const char *function, int line, GLUquadric* quad, GLboolean texture);
GLint OpenGL_gluScaleImage(const char *filename, const char *function, int line, GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut);
void OpenGL_gluSphere(const char *filename, const char *function, int line, GLUquadric* quad, GLdouble radius, GLint slices, GLint stacks);
void OpenGL_gluTessBeginContour(const char *filename, const char *function, int line, GLUtesselator* tess);
void OpenGL_gluTessBeginPolygon(const char *filename, const char *function, int line, GLUtesselator* tess, GLvoid* data);
void OpenGL_gluTessCallback(const char *filename, const char *function, int line, GLUtesselator* tess, GLenum which, _GLUfuncptr CallBackFunc);
void OpenGL_gluTessEndContour(const char *filename, const char *function, int line, GLUtesselator* tess);
void OpenGL_gluTessEndPolygon(const char *filename, const char *function, int line, GLUtesselator* tess);
void OpenGL_gluTessNormal(const char *filename, const char *function, int line, GLUtesselator* tess, GLdouble valueX, GLdouble valueY, GLdouble valueZ);
void OpenGL_gluTessProperty(const char *filename, const char *function, int line, GLUtesselator* tess, GLenum which, GLdouble data);
void OpenGL_gluTessVertex(const char *filename, const char *function, int line, GLUtesselator* tess, GLdouble *location, GLvoid* data);
GLint OpenGL_gluUnProject(const char *filename, const char *function, int line, GLdouble winX, GLdouble winY, GLdouble winZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* objX, GLdouble* objY, GLdouble* objZ);
GLint OpenGL_gluUnProject4(const char *filename, const char *function, int line, GLdouble winX, GLdouble winY, GLdouble winZ, GLdouble clipW, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble nearVal, GLdouble farVal, GLdouble* objX, GLdouble* objY, GLdouble* objZ, GLdouble* objW);

#ifndef DEBUG
#define glAccum(op,value) OpenGL_glAccum(__FILE__,FUNCTION,__LINE__,op,value)
#define glActiveTexture(texture) OpenGL_glActiveTexture(__FILE__,FUNCTION,__LINE__,texture)
#define glActiveTextureARB(texture) OpenGL_glActiveTextureARB(__FILE__,FUNCTION,__LINE__,texture)
#define glAlphaFunc(func,ref) OpenGL_glAlphaFunc(__FILE__,FUNCTION,__LINE__,func,ref)
#define glAreTexturesResident(n,textures,residences) OpenGL_glAreTexturesResident(__FILE__,FUNCTION,__LINE__,n,textures,residences)
#define glArrayElement(i) OpenGL_glArrayElement(__FILE__,FUNCTION,__LINE__,i)
#define glBindTexture(target,texture) OpenGL_glBindTexture(__FILE__,FUNCTION,__LINE__,target,texture)
#define glBitmap(width,height,xorig,yorig,xmove,ymove,bitmap) OpenGL_glBitmap(__FILE__,FUNCTION,__LINE__,width,height,xorig,yorig,xmove,ymove,bitmap)
#define glBlendColor(red,green,blue,alpha) OpenGL_glBlendColor(__FILE__,FUNCTION,__LINE__,red,green,blue,alpha)
#define glBlendEquation(mode) OpenGL_glBlendEquation(__FILE__,FUNCTION,__LINE__,mode)
#define glBlendFunc(sfactor,dfactor) OpenGL_glBlendFunc(__FILE__,FUNCTION,__LINE__,sfactor,dfactor)
#define glCallList(list) OpenGL_glCallList(__FILE__,FUNCTION,__LINE__,list)
#define glCallLists(n,type,lists) OpenGL_glCallLists(__FILE__,FUNCTION,__LINE__,n,type,lists)
#define glClear(mask) OpenGL_glClear(__FILE__,FUNCTION,__LINE__,mask)
#define glClearAccum(red,green,blue,alpha) OpenGL_glClearAccum(__FILE__,FUNCTION,__LINE__,red,green,blue,alpha)
#define glClearColor(red,green,blue,alpha) OpenGL_glClearColor(__FILE__,FUNCTION,__LINE__,red,green,blue,alpha)
#define glClearDepth(depth) OpenGL_glClearDepth(__FILE__,FUNCTION,__LINE__,depth)
#define glClearIndex(c) OpenGL_glClearIndex(__FILE__,FUNCTION,__LINE__,c)
#define glClearStencil(s) OpenGL_glClearStencil(__FILE__,FUNCTION,__LINE__,s)
#define glClientActiveTexture(texture) OpenGL_glClientActiveTexture(__FILE__,FUNCTION,__LINE__,texture)
#define glClientActiveTextureARB(texture) OpenGL_glClientActiveTextureARB(__FILE__,FUNCTION,__LINE__,texture)
#define glClipPlane(plane,equation) OpenGL_glClipPlane(__FILE__,FUNCTION,__LINE__,plane,equation)
#define glCompressedTexImage1D(target,level,internalformat,width,border,imageSize,data) OpenGL_glCompressedTexImage1D(__FILE__,FUNCTION,__LINE__,target,level,internalformat,width,border,imageSize,data)
#define glCompressedTexImage2D(target,level,internalformat,width,height,border,imageSize,data) OpenGL_glCompressedTexImage2D(__FILE__,FUNCTION,__LINE__,target,level,internalformat,width,height,border,imageSize,data)
#define glCompressedTexImage3D(target,level,internalformat,width,height,depth,border,imageSize,data) OpenGL_glCompressedTexImage3D(__FILE__,FUNCTION,__LINE__,target,level,internalformat,width,height,depth,border,imageSize,data)
#define glCompressedTexSubImage1D(target,level,xoffset,width,format,imageSize,data) OpenGL_glCompressedTexSubImage1D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,width,format,imageSize,data)
#define glCompressedTexSubImage2D(target,level,xoffset,yoffset,width,height,format,imageSize,data) OpenGL_glCompressedTexSubImage2D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,yoffset,width,height,format,imageSize,data)
#define glCompressedTexSubImage3D(target,level,xoffset,yoffset,zoffset,width,height,depth,format,imageSize,data) OpenGL_glCompressedTexSubImage3D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,yoffset,zoffset,width,height,depth,format,imageSize,data)
#define glConvolutionFilter1D(target,internalformat,width,format,type,image) OpenGL_glConvolutionFilter1D(__FILE__,FUNCTION,__LINE__,target,internalformat,width,format,type,image)
#define glConvolutionFilter2D(target,internalformat,width,height,format,type,image) OpenGL_glConvolutionFilter2D(__FILE__,FUNCTION,__LINE__,target,internalformat,width,height,format,type,image)
#define glConvolutionParameterf(target,pname,params) OpenGL_glConvolutionParameterf(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glConvolutionParameterfv(target,pname,params) OpenGL_glConvolutionParameterfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glConvolutionParameteri(target,pname,params) OpenGL_glConvolutionParameteri(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glConvolutionParameteriv(target,pname,params) OpenGL_glConvolutionParameteriv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glCopyColorSubTable(target,start,x,y,width) OpenGL_glCopyColorSubTable(__FILE__,FUNCTION,__LINE__,target,start,x,y,width)
#define glCopyColorTable(target,internalformat,x,y,width) OpenGL_glCopyColorTable(__FILE__,FUNCTION,__LINE__,target,internalformat,x,y,width)
#define glCopyConvolutionFilter1D(target,internalformat,x,y,width) OpenGL_glCopyConvolutionFilter1D(__FILE__,FUNCTION,__LINE__,target,internalformat,x,y,width)
#define glCopyConvolutionFilter2D(target,internalformat,x,y,width,height) OpenGL_glCopyConvolutionFilter2D(__FILE__,FUNCTION,__LINE__,target,internalformat,x,y,width,height)
#define glCopyPixels(x,y,width,height,type) OpenGL_glCopyPixels(__FILE__,FUNCTION,__LINE__,x,y,width,height,type)
#define glCopyTexImage1D(target,level,internalformat,x,y,width,border) OpenGL_glCopyTexImage1D(__FILE__,FUNCTION,__LINE__,target,level,internalformat,x,y,width,border)
#define glCopyTexImage2D(target,level,internalformat,x,y,width,height,border) OpenGL_glCopyTexImage2D(__FILE__,FUNCTION,__LINE__,target,level,internalformat,x,y,width,height,border)
#define glCopyTexSubImage1D(target,level,xoffset,x,y,width) OpenGL_glCopyTexSubImage1D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,x,y,width)
#define glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height) OpenGL_glCopyTexSubImage2D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,yoffset,x,y,width,height)
#define glCopyTexSubImage3D(target,level,xoffset,yoffset,zoffset,x,y,width,height) OpenGL_glCopyTexSubImage3D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,yoffset,zoffset,x,y,width,height)
#define glCullFace(mode) OpenGL_glCullFace(__FILE__,FUNCTION,__LINE__,mode)
#define glDeleteLists(list,range) OpenGL_glDeleteLists(__FILE__,FUNCTION,__LINE__,list,range)
#define glDeleteTextures(n,textures) OpenGL_glDeleteTextures(__FILE__,FUNCTION,__LINE__,n,textures)
#define glDepthFunc(func) OpenGL_glDepthFunc(__FILE__,FUNCTION,__LINE__,func)
#define glDepthMask(flag) OpenGL_glDepthMask(__FILE__,FUNCTION,__LINE__,flag)
#define glDepthRange(near_val,far_val) OpenGL_glDepthRange(__FILE__,FUNCTION,__LINE__,near_val,far_val)
#define glDisable(cap) OpenGL_glDisable(__FILE__,FUNCTION,__LINE__,cap)
#define glDisableClientState(cap) OpenGL_glDisableClientState(__FILE__,FUNCTION,__LINE__,cap)
#define glDrawArrays(mode,first,count) OpenGL_glDrawArrays(__FILE__,FUNCTION,__LINE__,mode,first,count)
#define glDrawBuffer(mode) OpenGL_glDrawBuffer(__FILE__,FUNCTION,__LINE__,mode)
#define glDrawElements(mode,count,type,indices) OpenGL_glDrawElements(__FILE__,FUNCTION,__LINE__,mode,count,type,indices)
#define glDrawPixels(width,height,format,type,pixels) OpenGL_glDrawPixels(__FILE__,FUNCTION,__LINE__,width,height,format,type,pixels)
#define glDrawRangeElements(mode,start,end,count,type,indices) OpenGL_glDrawRangeElements(__FILE__,FUNCTION,__LINE__,mode,start,end,count,type,indices)
#define glEdgeFlag(flag) OpenGL_glEdgeFlag(__FILE__,FUNCTION,__LINE__,flag)
#define glEdgeFlagPointer(stride,ptr) OpenGL_glEdgeFlagPointer(__FILE__,FUNCTION,__LINE__,stride,ptr)
#define glEdgeFlagv(flag) OpenGL_glEdgeFlagv(__FILE__,FUNCTION,__LINE__,flag)
#define glEnable(cap) OpenGL_glEnable(__FILE__,FUNCTION,__LINE__,cap)
#define glEnableClientState(cap) OpenGL_glEnableClientState(__FILE__,FUNCTION,__LINE__,cap)
#define glEvalCoord1d(u) OpenGL_glEvalCoord1d(__FILE__,FUNCTION,__LINE__,u)
#define glEvalCoord1dv(u) OpenGL_glEvalCoord1dv(__FILE__,FUNCTION,__LINE__,u)
#define glEvalCoord1f(u) OpenGL_glEvalCoord1f(__FILE__,FUNCTION,__LINE__,u)
#define glEvalCoord1fv(u) OpenGL_glEvalCoord1fv(__FILE__,FUNCTION,__LINE__,u)
#define glEvalCoord2d(u,v) OpenGL_glEvalCoord2d(__FILE__,FUNCTION,__LINE__,u,v)
#define glEvalCoord2dv(u) OpenGL_glEvalCoord2dv(__FILE__,FUNCTION,__LINE__,u)
#define glEvalCoord2f(u,v) OpenGL_glEvalCoord2f(__FILE__,FUNCTION,__LINE__,u,v)
#define glEvalCoord2fv(u) OpenGL_glEvalCoord2fv(__FILE__,FUNCTION,__LINE__,u)
#define glEvalMesh1(mode,i1,i2) OpenGL_glEvalMesh1(__FILE__,FUNCTION,__LINE__,mode,i1,i2)
#define glEvalMesh2(mode,i1,i2,j1,j2) OpenGL_glEvalMesh2(__FILE__,FUNCTION,__LINE__,mode,i1,i2,j1,j2)
#define glEvalPoint1(i) OpenGL_glEvalPoint1(__FILE__,FUNCTION,__LINE__,i)
#define glEvalPoint2(i,j) OpenGL_glEvalPoint2(__FILE__,FUNCTION,__LINE__,i,j)
#define glFeedbackBuffer(size,type,buffer) OpenGL_glFeedbackBuffer(__FILE__,FUNCTION,__LINE__,size,type,buffer)
#define glFinish(void) OpenGL_glFinish(__FILE__,FUNCTION,__LINE__)
#define glFlush(void) OpenGL_glFlush(__FILE__,FUNCTION,__LINE__)
#define glFogf(pname,param) OpenGL_glFogf(__FILE__,FUNCTION,__LINE__,pname,param)
#define glFogfv(pname,params) OpenGL_glFogfv(__FILE__,FUNCTION,__LINE__,pname,params)
#define glFogi(pname,param) OpenGL_glFogi(__FILE__,FUNCTION,__LINE__,pname,param)
#define glFogiv(pname,params) OpenGL_glFogiv(__FILE__,FUNCTION,__LINE__,pname,params)
#define glFrontFace(mode) OpenGL_glFrontFace(__FILE__,FUNCTION,__LINE__,mode)
#define glFrustum(left,right,bottom,top,near_val,far_val) OpenGL_glFrustum(__FILE__,FUNCTION,__LINE__,left,right,bottom,top,near_val,far_val)
#define glGenLists(range) OpenGL_glGenLists(__FILE__,FUNCTION,__LINE__,range)
#define glGenTextures(n,textures) OpenGL_glGenTextures(__FILE__,FUNCTION,__LINE__,n,textures)
#define glGetBooleanv(pname,params) OpenGL_glGetBooleanv(__FILE__,FUNCTION,__LINE__,pname,params)
#define glGetClipPlane(plane,equation) OpenGL_glGetClipPlane(__FILE__,FUNCTION,__LINE__,plane,equation)
#define glGetColorTable(target,format,type,table) OpenGL_glGetColorTable(__FILE__,FUNCTION,__LINE__,target,format,type,table)
#define glGetColorTableParameterfv(target,pname,params) OpenGL_glGetColorTableParameterfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetColorTableParameteriv(target,pname,params) OpenGL_glGetColorTableParameteriv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetCompressedTexImage(target,lod,img) OpenGL_glGetCompressedTexImage(__FILE__,FUNCTION,__LINE__,target,lod,img)
#define glGetConvolutionFilter(target,format,type,image) OpenGL_glGetConvolutionFilter(__FILE__,FUNCTION,__LINE__,target,format,type,image)
#define glGetConvolutionParameterfv(target,pname,params) OpenGL_glGetConvolutionParameterfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetConvolutionParameteriv(target,pname,params) OpenGL_glGetConvolutionParameteriv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetDoublev(pname,params) OpenGL_glGetDoublev(__FILE__,FUNCTION,__LINE__,pname,params)
#define glGetError(void) OpenGL_glGetError(__FILE__,FUNCTION,__LINE__)
#define glGetFloatv(pname,params) OpenGL_glGetFloatv(__FILE__,FUNCTION,__LINE__,pname,params)
#define glGetHistogram(target,reset,format,type,values) OpenGL_glGetHistogram(__FILE__,FUNCTION,__LINE__,target,reset,format,type,values)
#define glGetHistogramParameterfv(target,pname,params) OpenGL_glGetHistogramParameterfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetHistogramParameteriv(target,pname,params) OpenGL_glGetHistogramParameteriv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetIntegerv(pname,params) OpenGL_glGetIntegerv(__FILE__,FUNCTION,__LINE__,pname,params)
#define glGetLightfv(light,pname,params) OpenGL_glGetLightfv(__FILE__,FUNCTION,__LINE__,light,pname,params)
#define glGetLightiv(light,pname,params) OpenGL_glGetLightiv(__FILE__,FUNCTION,__LINE__,light,pname,params)
#define glGetMapdv(target,query,v) OpenGL_glGetMapdv(__FILE__,FUNCTION,__LINE__,target,query,v)
#define glGetMapfv(target,query,v) OpenGL_glGetMapfv(__FILE__,FUNCTION,__LINE__,target,query,v)
#define glGetMapiv(target,query,v) OpenGL_glGetMapiv(__FILE__,FUNCTION,__LINE__,target,query,v)
#define glGetMaterialfv(face,pname,params) OpenGL_glGetMaterialfv(__FILE__,FUNCTION,__LINE__,face,pname,params)
#define glGetMaterialiv(face,pname,params) OpenGL_glGetMaterialiv(__FILE__,FUNCTION,__LINE__,face,pname,params)
#define glGetMinmax(target,reset,format,types,values) OpenGL_glGetMinmax(__FILE__,FUNCTION,__LINE__,target,reset,format,types,values)
#define glGetMinmaxParameterfv(target,pname,params) OpenGL_glGetMinmaxParameterfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetMinmaxParameteriv(target,pname,params) OpenGL_glGetMinmaxParameteriv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetPixelMapfv(map,values) OpenGL_glGetPixelMapfv(__FILE__,FUNCTION,__LINE__,map,values)
#define glGetPixelMapuiv(map,values) OpenGL_glGetPixelMapuiv(__FILE__,FUNCTION,__LINE__,map,values)
#define glGetPixelMapusv(map,values) OpenGL_glGetPixelMapusv(__FILE__,FUNCTION,__LINE__,map,values)
#define glGetPointerv(pname,params) OpenGL_glGetPointerv(__FILE__,FUNCTION,__LINE__,pname,params)
#define glGetPolygonStipple(mask) OpenGL_glGetPolygonStipple(__FILE__,FUNCTION,__LINE__,mask)
#define glGetSeparableFilter(target,format,type,row,column,span) OpenGL_glGetSeparableFilter(__FILE__,FUNCTION,__LINE__,target,format,type,row,column,span)
#define glGetString(name) OpenGL_glGetString(__FILE__,FUNCTION,__LINE__,name)
#define glGetTexEnvfv(target,pname,params) OpenGL_glGetTexEnvfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetTexEnviv(target,pname,params) OpenGL_glGetTexEnviv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetTexGendv(coord,pname,params) OpenGL_glGetTexGendv(__FILE__,FUNCTION,__LINE__,coord,pname,params)
#define glGetTexGenfv(coord,pname,params) OpenGL_glGetTexGenfv(__FILE__,FUNCTION,__LINE__,coord,pname,params)
#define glGetTexGeniv(coord,pname,params) OpenGL_glGetTexGeniv(__FILE__,FUNCTION,__LINE__,coord,pname,params)
#define glGetTexImage(target,level,format,type,pixels) OpenGL_glGetTexImage(__FILE__,FUNCTION,__LINE__,target,level,format,type,pixels)
#define glGetTexLevelParameterfv(target,level,pname,params) OpenGL_glGetTexLevelParameterfv(__FILE__,FUNCTION,__LINE__,target,level,pname,params)
#define glGetTexLevelParameteriv(target,level,pname,params) OpenGL_glGetTexLevelParameteriv(__FILE__,FUNCTION,__LINE__,target,level,pname,params)
#define glGetTexParameterfv(target,pname,params) OpenGL_glGetTexParameterfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glGetTexParameteriv(target,pname,params) OpenGL_glGetTexParameteriv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glHint(target,mode) OpenGL_glHint(__FILE__,FUNCTION,__LINE__,target,mode)
#define glHistogram(target,width,internalformat,sink) OpenGL_glHistogram(__FILE__,FUNCTION,__LINE__,target,width,internalformat,sink)
#define glIndexMask(mask) OpenGL_glIndexMask(__FILE__,FUNCTION,__LINE__,mask)
#define glIndexPointer(type,stride,ptr) OpenGL_glIndexPointer(__FILE__,FUNCTION,__LINE__,type,stride,ptr)
#define glIndexd(c) OpenGL_glIndexd(__FILE__,FUNCTION,__LINE__,c)
#define glIndexdv(c) OpenGL_glIndexdv(__FILE__,FUNCTION,__LINE__,c)
#define glIndexf(c) OpenGL_glIndexf(__FILE__,FUNCTION,__LINE__,c)
#define glIndexfv(c) OpenGL_glIndexfv(__FILE__,FUNCTION,__LINE__,c)
#define glIndexi(c) OpenGL_glIndexi(__FILE__,FUNCTION,__LINE__,c)
#define glIndexiv(c) OpenGL_glIndexiv(__FILE__,FUNCTION,__LINE__,c)
#define glIndexs(c) OpenGL_glIndexs(__FILE__,FUNCTION,__LINE__,c)
#define glIndexsv(c) OpenGL_glIndexsv(__FILE__,FUNCTION,__LINE__,c)
#define glIndexub(c) OpenGL_glIndexub(__FILE__,FUNCTION,__LINE__,c)
#define glIndexubv(c) OpenGL_glIndexubv(__FILE__,FUNCTION,__LINE__,c)
#define glInitNames(void) OpenGL_glInitNames(__FILE__,FUNCTION,__LINE__)
#define glInterleavedArrays(format,stride,pointer) OpenGL_glInterleavedArrays(__FILE__,FUNCTION,__LINE__,format,stride,pointer)
#define glIsEnabled(cap) OpenGL_glIsEnabled(__FILE__,FUNCTION,__LINE__,cap)
#define glIsList(list) OpenGL_glIsList(__FILE__,FUNCTION,__LINE__,list)
#define glIsTexture(texture) OpenGL_glIsTexture(__FILE__,FUNCTION,__LINE__,texture)
#define glLightModelf(pname,param) OpenGL_glLightModelf(__FILE__,FUNCTION,__LINE__,pname,param)
#define glLightModelfv(pname,params) OpenGL_glLightModelfv(__FILE__,FUNCTION,__LINE__,pname,params)
#define glLightModeli(pname,param) OpenGL_glLightModeli(__FILE__,FUNCTION,__LINE__,pname,param)
#define glLightModeliv(pname,params) OpenGL_glLightModeliv(__FILE__,FUNCTION,__LINE__,pname,params)
#define glLightf(light,pname,param) OpenGL_glLightf(__FILE__,FUNCTION,__LINE__,light,pname,param)
#define glLightfv(light,pname,params) OpenGL_glLightfv(__FILE__,FUNCTION,__LINE__,light,pname,params)
#define glLighti(light,pname,param) OpenGL_glLighti(__FILE__,FUNCTION,__LINE__,light,pname,param)
#define glLightiv(light,pname,params) OpenGL_glLightiv(__FILE__,FUNCTION,__LINE__,light,pname,params)
#define glLineStipple(factor,pattern) OpenGL_glLineStipple(__FILE__,FUNCTION,__LINE__,factor,pattern)
#define glLineWidth(width) OpenGL_glLineWidth(__FILE__,FUNCTION,__LINE__,width)
#define glListBase(base) OpenGL_glListBase(__FILE__,FUNCTION,__LINE__,base)
#define glLoadIdentity(void) OpenGL_glLoadIdentity(__FILE__,FUNCTION,__LINE__)
#define glLoadMatrixd(m) OpenGL_glLoadMatrixd(__FILE__,FUNCTION,__LINE__,m)
#define glLoadMatrixf(m) OpenGL_glLoadMatrixf(__FILE__,FUNCTION,__LINE__,m)
#define glLoadName(name) OpenGL_glLoadName(__FILE__,FUNCTION,__LINE__,name)
#define glLoadTransposeMatrixd(m) OpenGL_glLoadTransposeMatrixd(__FILE__,FUNCTION,__LINE__,m)
#define glLoadTransposeMatrixf(m) OpenGL_glLoadTransposeMatrixf(__FILE__,FUNCTION,__LINE__,m)
#define glLogicOp(opcode) OpenGL_glLogicOp(__FILE__,FUNCTION,__LINE__,opcode)
#define glMap1d(target,u1,u2,stride,order,points) OpenGL_glMap1d(__FILE__,FUNCTION,__LINE__,target,u1,u2,stride,order,points)
#define glMap1f(target,u1,u2,stride,order,points) OpenGL_glMap1f(__FILE__,FUNCTION,__LINE__,target,u1,u2,stride,order,points)
#define glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points) OpenGL_glMap2d(__FILE__,FUNCTION,__LINE__,target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points)
#define glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points) OpenGL_glMap2f(__FILE__,FUNCTION,__LINE__,target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points)
#define glMapGrid1d(un,u1,u2) OpenGL_glMapGrid1d(__FILE__,FUNCTION,__LINE__,un,u1,u2)
#define glMapGrid1f(un,u1,u2) OpenGL_glMapGrid1f(__FILE__,FUNCTION,__LINE__,un,u1,u2)
#define glMapGrid2d(un,u1,u2,vn,v1,v2) OpenGL_glMapGrid2d(__FILE__,FUNCTION,__LINE__,un,u1,u2,vn,v1,v2)
#define glMapGrid2f(un,u1,u2,vn,v1,v2) OpenGL_glMapGrid2f(__FILE__,FUNCTION,__LINE__,un,u1,u2,vn,v1,v2)
#define glMaterialf(face,pname,param) OpenGL_glMaterialf(__FILE__,FUNCTION,__LINE__,face,pname,param)
#define glMaterialfv(face,pname,params) OpenGL_glMaterialfv(__FILE__,FUNCTION,__LINE__,face,pname,params)
#define glMateriali(face,pname,param) OpenGL_glMateriali(__FILE__,FUNCTION,__LINE__,face,pname,param)
#define glMaterialiv(face,pname,params) OpenGL_glMaterialiv(__FILE__,FUNCTION,__LINE__,face,pname,params)
#define glMatrixMode(mode) OpenGL_glMatrixMode(__FILE__,FUNCTION,__LINE__,mode)
#define glMinmax(target,internalformat,sink) OpenGL_glMinmax(__FILE__,FUNCTION,__LINE__,target,internalformat,sink)
#define glMultMatrixd(m) OpenGL_glMultMatrixd(__FILE__,FUNCTION,__LINE__,m)
#define glMultMatrixf(m) OpenGL_glMultMatrixf(__FILE__,FUNCTION,__LINE__,m)
#define glMultTransposeMatrixd(m) OpenGL_glMultTransposeMatrixd(__FILE__,FUNCTION,__LINE__,m)
#define glMultTransposeMatrixf(m) OpenGL_glMultTransposeMatrixf(__FILE__,FUNCTION,__LINE__,m)
#define glMultiTexCoord1d(target,s) OpenGL_glMultiTexCoord1d(__FILE__,FUNCTION,__LINE__,target,s)
#define glMultiTexCoord1dARB(target,s) OpenGL_glMultiTexCoord1dARB(__FILE__,FUNCTION,__LINE__,target,s)
#define glMultiTexCoord1dv(target,v) OpenGL_glMultiTexCoord1dv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord1dvARB(target,v) OpenGL_glMultiTexCoord1dvARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord1f(target,s) OpenGL_glMultiTexCoord1f(__FILE__,FUNCTION,__LINE__,target,s)
#define glMultiTexCoord1fARB(target,s) OpenGL_glMultiTexCoord1fARB(__FILE__,FUNCTION,__LINE__,target,s)
#define glMultiTexCoord1fv(target,v) OpenGL_glMultiTexCoord1fv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord1fvARB(target,v) OpenGL_glMultiTexCoord1fvARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord1i(target,s) OpenGL_glMultiTexCoord1i(__FILE__,FUNCTION,__LINE__,target,s)
#define glMultiTexCoord1iARB(target,s) OpenGL_glMultiTexCoord1iARB(__FILE__,FUNCTION,__LINE__,target,s)
#define glMultiTexCoord1iv(target,v) OpenGL_glMultiTexCoord1iv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord1ivARB(target,v) OpenGL_glMultiTexCoord1ivARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord1s(target,s) OpenGL_glMultiTexCoord1s(__FILE__,FUNCTION,__LINE__,target,s)
#define glMultiTexCoord1sARB(target,s) OpenGL_glMultiTexCoord1sARB(__FILE__,FUNCTION,__LINE__,target,s)
#define glMultiTexCoord1sv(target,v) OpenGL_glMultiTexCoord1sv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord1svARB(target,v) OpenGL_glMultiTexCoord1svARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord2d(target,s,t) OpenGL_glMultiTexCoord2d(__FILE__,FUNCTION,__LINE__,target,s,t)
#define glMultiTexCoord2dARB(target,s,t) OpenGL_glMultiTexCoord2dARB(__FILE__,FUNCTION,__LINE__,target,s,t)
#define glMultiTexCoord2dv(target,v) OpenGL_glMultiTexCoord2dv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord2dvARB(target,v) OpenGL_glMultiTexCoord2dvARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord2f(target,s,t) OpenGL_glMultiTexCoord2f(__FILE__,FUNCTION,__LINE__,target,s,t)
#define glMultiTexCoord2fARB(target,s,t) OpenGL_glMultiTexCoord2fARB(__FILE__,FUNCTION,__LINE__,target,s,t)
#define glMultiTexCoord2fv(target,v) OpenGL_glMultiTexCoord2fv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord2fvARB(target,v) OpenGL_glMultiTexCoord2fvARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord2i(target,s,t) OpenGL_glMultiTexCoord2i(__FILE__,FUNCTION,__LINE__,target,s,t)
#define glMultiTexCoord2iARB(target,s,t) OpenGL_glMultiTexCoord2iARB(__FILE__,FUNCTION,__LINE__,target,s,t)
#define glMultiTexCoord2iv(target,v) OpenGL_glMultiTexCoord2iv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord2ivARB(target,v) OpenGL_glMultiTexCoord2ivARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord2s(target,s,t) OpenGL_glMultiTexCoord2s(__FILE__,FUNCTION,__LINE__,target,s,t)
#define glMultiTexCoord2sARB(target,s,t) OpenGL_glMultiTexCoord2sARB(__FILE__,FUNCTION,__LINE__,target,s,t)
#define glMultiTexCoord2sv(target,v) OpenGL_glMultiTexCoord2sv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord2svARB(target,v) OpenGL_glMultiTexCoord2svARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord3d(target,s,t,r) OpenGL_glMultiTexCoord3d(__FILE__,FUNCTION,__LINE__,target,s,t,r)
#define glMultiTexCoord3dARB(target,s,t,r) OpenGL_glMultiTexCoord3dARB(__FILE__,FUNCTION,__LINE__,target,s,t,r)
#define glMultiTexCoord3dv(target,v) OpenGL_glMultiTexCoord3dv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord3dvARB(target,v) OpenGL_glMultiTexCoord3dvARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord3f(target,s,t,r) OpenGL_glMultiTexCoord3f(__FILE__,FUNCTION,__LINE__,target,s,t,r)
#define glMultiTexCoord3fARB(target,s,t,r) OpenGL_glMultiTexCoord3fARB(__FILE__,FUNCTION,__LINE__,target,s,t,r)
#define glMultiTexCoord3fv(target,v) OpenGL_glMultiTexCoord3fv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord3fvARB(target,v) OpenGL_glMultiTexCoord3fvARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord3i(target,s,t,r) OpenGL_glMultiTexCoord3i(__FILE__,FUNCTION,__LINE__,target,s,t,r)
#define glMultiTexCoord3iARB(target,s,t,r) OpenGL_glMultiTexCoord3iARB(__FILE__,FUNCTION,__LINE__,target,s,t,r)
#define glMultiTexCoord3iv(target,v) OpenGL_glMultiTexCoord3iv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord3ivARB(target,v) OpenGL_glMultiTexCoord3ivARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord3s(target,s,t,r) OpenGL_glMultiTexCoord3s(__FILE__,FUNCTION,__LINE__,target,s,t,r)
#define glMultiTexCoord3sARB(target,s,t,r) OpenGL_glMultiTexCoord3sARB(__FILE__,FUNCTION,__LINE__,target,s,t,r)
#define glMultiTexCoord3sv(target,v) OpenGL_glMultiTexCoord3sv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord3svARB(target,v) OpenGL_glMultiTexCoord3svARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord4d(target,s,t,r,q) OpenGL_glMultiTexCoord4d(__FILE__,FUNCTION,__LINE__,target,s,t,r,q)
#define glMultiTexCoord4dARB(target,s,t,r,q) OpenGL_glMultiTexCoord4dARB(__FILE__,FUNCTION,__LINE__,target,s,t,r,q)
#define glMultiTexCoord4dv(target,v) OpenGL_glMultiTexCoord4dv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord4dvARB(target,v) OpenGL_glMultiTexCoord4dvARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord4f(target,s,t,r,q) OpenGL_glMultiTexCoord4f(__FILE__,FUNCTION,__LINE__,target,s,t,r,q)
#define glMultiTexCoord4fARB(target,s,t,r,q) OpenGL_glMultiTexCoord4fARB(__FILE__,FUNCTION,__LINE__,target,s,t,r,q)
#define glMultiTexCoord4fv(target,v) OpenGL_glMultiTexCoord4fv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord4fvARB(target,v) OpenGL_glMultiTexCoord4fvARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord4i(target,s,t,r,q) OpenGL_glMultiTexCoord4i(__FILE__,FUNCTION,__LINE__,target,s,t,r,q)
#define glMultiTexCoord4iARB(target,s,t,r,q) OpenGL_glMultiTexCoord4iARB(__FILE__,FUNCTION,__LINE__,target,s,t,r,q)
#define glMultiTexCoord4iv(target,v) OpenGL_glMultiTexCoord4iv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord4ivARB(target,v) OpenGL_glMultiTexCoord4ivARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord4s(target,s,t,r,q) OpenGL_glMultiTexCoord4s(__FILE__,FUNCTION,__LINE__,target,s,t,r,q)
#define glMultiTexCoord4sARB(target,s,t,r,q) OpenGL_glMultiTexCoord4sARB(__FILE__,FUNCTION,__LINE__,target,s,t,r,q)
#define glMultiTexCoord4sv(target,v) OpenGL_glMultiTexCoord4sv(__FILE__,FUNCTION,__LINE__,target,v)
#define glMultiTexCoord4svARB(target,v) OpenGL_glMultiTexCoord4svARB(__FILE__,FUNCTION,__LINE__,target,v)
#define glNewList(list,mode) OpenGL_glNewList(__FILE__,FUNCTION,__LINE__,list,mode)
#define glOrtho(left,right,bottom,top,near_val,far_val) OpenGL_glOrtho(__FILE__,FUNCTION,__LINE__,left,right,bottom,top,near_val,far_val)
#define glPassThrough(token) OpenGL_glPassThrough(__FILE__,FUNCTION,__LINE__,token)
#define glPixelMapfv(map,mapsize,values) OpenGL_glPixelMapfv(__FILE__,FUNCTION,__LINE__,map,mapsize,values)
#define glPixelMapuiv(map,mapsize,values) OpenGL_glPixelMapuiv(__FILE__,FUNCTION,__LINE__,map,mapsize,values)
#define glPixelMapusv(map,mapsize,values) OpenGL_glPixelMapusv(__FILE__,FUNCTION,__LINE__,map,mapsize,values)
#define glPixelStoref(pname,param) OpenGL_glPixelStoref(__FILE__,FUNCTION,__LINE__,pname,param)
#define glPixelStorei(pname,param) OpenGL_glPixelStorei(__FILE__,FUNCTION,__LINE__,pname,param)
#define glPixelTransferf(pname,param) OpenGL_glPixelTransferf(__FILE__,FUNCTION,__LINE__,pname,param)
#define glPixelTransferi(pname,param) OpenGL_glPixelTransferi(__FILE__,FUNCTION,__LINE__,pname,param)
#define glPixelZoom(xfactor,yfactor) OpenGL_glPixelZoom(__FILE__,FUNCTION,__LINE__,xfactor,yfactor)
#define glPointSize(size) OpenGL_glPointSize(__FILE__,FUNCTION,__LINE__,size)
#define glPolygonMode(face,mode) OpenGL_glPolygonMode(__FILE__,FUNCTION,__LINE__,face,mode)
#define glPolygonOffset(factor,units) OpenGL_glPolygonOffset(__FILE__,FUNCTION,__LINE__,factor,units)
#define glPolygonStipple(mask) OpenGL_glPolygonStipple(__FILE__,FUNCTION,__LINE__,mask)
#define glPopAttrib(void) OpenGL_glPopAttrib(__FILE__,FUNCTION,__LINE__)
#define glPopClientAttrib(void) OpenGL_glPopClientAttrib(__FILE__,FUNCTION,__LINE__)
#define glPopMatrix(void) OpenGL_glPopMatrix(__FILE__,FUNCTION,__LINE__)
#define glPopName(void) OpenGL_glPopName(__FILE__,FUNCTION,__LINE__)
#define glPrioritizeTextures(n,textures,priorities) OpenGL_glPrioritizeTextures(__FILE__,FUNCTION,__LINE__,n,textures,priorities)
#define glPushAttrib(mask) OpenGL_glPushAttrib(__FILE__,FUNCTION,__LINE__,mask)
#define glPushClientAttrib(mask) OpenGL_glPushClientAttrib(__FILE__,FUNCTION,__LINE__,mask)
#define glPushMatrix(void) OpenGL_glPushMatrix(__FILE__,FUNCTION,__LINE__)
#define glPushName(name) OpenGL_glPushName(__FILE__,FUNCTION,__LINE__,name)
#define glRasterPos2d(x,y) OpenGL_glRasterPos2d(__FILE__,FUNCTION,__LINE__,x,y)
#define glRasterPos2dv(v) OpenGL_glRasterPos2dv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos2f(x,y) OpenGL_glRasterPos2f(__FILE__,FUNCTION,__LINE__,x,y)
#define glRasterPos2fv(v) OpenGL_glRasterPos2fv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos2i(x,y) OpenGL_glRasterPos2i(__FILE__,FUNCTION,__LINE__,x,y)
#define glRasterPos2iv(v) OpenGL_glRasterPos2iv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos2s(x,y) OpenGL_glRasterPos2s(__FILE__,FUNCTION,__LINE__,x,y)
#define glRasterPos2sv(v) OpenGL_glRasterPos2sv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos3d(x,y,z) OpenGL_glRasterPos3d(__FILE__,FUNCTION,__LINE__,x,y,z)
#define glRasterPos3dv(v) OpenGL_glRasterPos3dv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos3f(x,y,z) OpenGL_glRasterPos3f(__FILE__,FUNCTION,__LINE__,x,y,z)
#define glRasterPos3fv(v) OpenGL_glRasterPos3fv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos3i(x,y,z) OpenGL_glRasterPos3i(__FILE__,FUNCTION,__LINE__,x,y,z)
#define glRasterPos3iv(v) OpenGL_glRasterPos3iv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos3s(x,y,z) OpenGL_glRasterPos3s(__FILE__,FUNCTION,__LINE__,x,y,z)
#define glRasterPos3sv(v) OpenGL_glRasterPos3sv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos4d(x,y,z,w) OpenGL_glRasterPos4d(__FILE__,FUNCTION,__LINE__,x,y,z,w)
#define glRasterPos4dv(v) OpenGL_glRasterPos4dv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos4f(x,y,z,w) OpenGL_glRasterPos4f(__FILE__,FUNCTION,__LINE__,x,y,z,w)
#define glRasterPos4fv(v) OpenGL_glRasterPos4fv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos4i(x,y,z,w) OpenGL_glRasterPos4i(__FILE__,FUNCTION,__LINE__,x,y,z,w)
#define glRasterPos4iv(v) OpenGL_glRasterPos4iv(__FILE__,FUNCTION,__LINE__,v)
#define glRasterPos4s(x,y,z,w) OpenGL_glRasterPos4s(__FILE__,FUNCTION,__LINE__,x,y,z,w)
#define glRasterPos4sv(v) OpenGL_glRasterPos4sv(__FILE__,FUNCTION,__LINE__,v)
#define glReadBuffer(mode) OpenGL_glReadBuffer(__FILE__,FUNCTION,__LINE__,mode)
#define glReadPixels(x,y,width,height,format,type,pixels) OpenGL_glReadPixels(__FILE__,FUNCTION,__LINE__,x,y,width,height,format,type,pixels)
#define glRectd(x1,y1,x2,y2) OpenGL_glRectd(__FILE__,FUNCTION,__LINE__,x1,y1,x2,y2)
#define glRectdv(v1,v2) OpenGL_glRectdv(__FILE__,FUNCTION,__LINE__,v1,v2)
#define glRectf(x1,y1,x2,y2) OpenGL_glRectf(__FILE__,FUNCTION,__LINE__,x1,y1,x2,y2)
#define glRectfv(v1,v2) OpenGL_glRectfv(__FILE__,FUNCTION,__LINE__,v1,v2)
#define glRecti(x1,y1,x2,y2) OpenGL_glRecti(__FILE__,FUNCTION,__LINE__,x1,y1,x2,y2)
#define glRectiv(v1,v2) OpenGL_glRectiv(__FILE__,FUNCTION,__LINE__,v1,v2)
#define glRects(x1,y1,x2,y2) OpenGL_glRects(__FILE__,FUNCTION,__LINE__,x1,y1,x2,y2)
#define glRectsv(v1,v2) OpenGL_glRectsv(__FILE__,FUNCTION,__LINE__,v1,v2)
#define glRenderMode(mode) OpenGL_glRenderMode(__FILE__,FUNCTION,__LINE__,mode)
#define glResetHistogram(target) OpenGL_glResetHistogram(__FILE__,FUNCTION,__LINE__,target)
#define glResetMinmax(target) OpenGL_glResetMinmax(__FILE__,FUNCTION,__LINE__,target)
#define glRotated(angle,x,y,z) OpenGL_glRotated(__FILE__,FUNCTION,__LINE__,angle,x,y,z)
#define glRotatef(angle,x,y,z) OpenGL_glRotatef(__FILE__,FUNCTION,__LINE__,angle,x,y,z)
#define glSampleCoverage(value,invert) OpenGL_glSampleCoverage(__FILE__,FUNCTION,__LINE__,value,invert)
#define glScaled(x,y,z) OpenGL_glScaled(__FILE__,FUNCTION,__LINE__,x,y,z)
#define glScalef(x,y,z) OpenGL_glScalef(__FILE__,FUNCTION,__LINE__,x,y,z)
#define glScissor(x,y,width,height) OpenGL_glScissor(__FILE__,FUNCTION,__LINE__,x,y,width,height)
#define glSelectBuffer(size,buffer) OpenGL_glSelectBuffer(__FILE__,FUNCTION,__LINE__,size,buffer)
#define glSeparableFilter2D(target,internalformat,width,height,format,type,row,column) OpenGL_glSeparableFilter2D(__FILE__,FUNCTION,__LINE__,target,internalformat,width,height,format,type,row,column)
#define glShadeModel(mode) OpenGL_glShadeModel(__FILE__,FUNCTION,__LINE__,mode)
#define glStencilFunc(func,ref,mask) OpenGL_glStencilFunc(__FILE__,FUNCTION,__LINE__,func,ref,mask)
#define glStencilMask(mask) OpenGL_glStencilMask(__FILE__,FUNCTION,__LINE__,mask)
#define glStencilOp(fail,zfail,zpass) OpenGL_glStencilOp(__FILE__,FUNCTION,__LINE__,fail,zfail,zpass)
#define glTexEnvf(target,pname,param) OpenGL_glTexEnvf(__FILE__,FUNCTION,__LINE__,target,pname,param)
#define glTexEnvfv(target,pname,params) OpenGL_glTexEnvfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glTexEnvi(target,pname,param) OpenGL_glTexEnvi(__FILE__,FUNCTION,__LINE__,target,pname,param)
#define glTexEnviv(target,pname,params) OpenGL_glTexEnviv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glTexGend(coord,pname,param) OpenGL_glTexGend(__FILE__,FUNCTION,__LINE__,coord,pname,param)
#define glTexGendv(coord,pname,params) OpenGL_glTexGendv(__FILE__,FUNCTION,__LINE__,coord,pname,params)
#define glTexGenf(coord,pname,param) OpenGL_glTexGenf(__FILE__,FUNCTION,__LINE__,coord,pname,param)
#define glTexGenfv(coord,pname,params) OpenGL_glTexGenfv(__FILE__,FUNCTION,__LINE__,coord,pname,params)
#define glTexGeni(coord,pname,param) OpenGL_glTexGeni(__FILE__,FUNCTION,__LINE__,coord,pname,param)
#define glTexGeniv(coord,pname,params) OpenGL_glTexGeniv(__FILE__,FUNCTION,__LINE__,coord,pname,params)
#define glTexImage1D(target,level,internalFormat,width,border,format,type,pixels) OpenGL_glTexImage1D(__FILE__,FUNCTION,__LINE__,target,level,internalFormat,width,border,format,type,pixels)
#define glTexImage2D(target,level,internalFormat,width,height,border,format,type,pixels) OpenGL_glTexImage2D(__FILE__,FUNCTION,__LINE__,target,level,internalFormat,width,height,border,format,type,pixels)
#define glTexImage3D(target,level,internalFormat,width,height,depth,border,format,type,pixels) OpenGL_glTexImage3D(__FILE__,FUNCTION,__LINE__,target,level,internalFormat,width,height,depth,border,format,type,pixels)
#define glTexParameterf(target,pname,param) OpenGL_glTexParameterf(__FILE__,FUNCTION,__LINE__,target,pname,param)
#define glTexParameterfv(target,pname,params) OpenGL_glTexParameterfv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glTexParameteri(target,pname,param) OpenGL_glTexParameteri(__FILE__,FUNCTION,__LINE__,target,pname,param)
#define glTexParameteriv(target,pname,params) OpenGL_glTexParameteriv(__FILE__,FUNCTION,__LINE__,target,pname,params)
#define glTexSubImage1D(target,level,xoffset,width,format,type,pixels) OpenGL_glTexSubImage1D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,width,format,type,pixels)
#define glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels) OpenGL_glTexSubImage2D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,yoffset,width,height,format,type,pixels)
#define glTexSubImage3D(target,level,xoffset,yoffset,zoffset,width,height,depth,format,type,pixels) OpenGL_glTexSubImage3D(__FILE__,FUNCTION,__LINE__,target,level,xoffset,yoffset,zoffset,width,height,depth,format,type,pixels)
#define glTranslated(x,y,z) OpenGL_glTranslated(__FILE__,FUNCTION,__LINE__,x,y,z)
#define glTranslatef(x,y,z) OpenGL_glTranslatef(__FILE__,FUNCTION,__LINE__,x,y,z)
#define glViewport(x,y,width,height) OpenGL_glViewport(__FILE__,FUNCTION,__LINE__,x,y,width,height)
#define gluBeginCurve(nurb) OpenGL_gluBeginCurve(__FILE__,FUNCTION,__LINE__,nurb)
#define gluBeginPolygon(tess) OpenGL_gluBeginPolygon(__FILE__,FUNCTION,__LINE__,tess)
#define gluBeginSurface(nurb) OpenGL_gluBeginSurface(__FILE__,FUNCTION,__LINE__,nurb)
#define gluBeginTrim(nurb) OpenGL_gluBeginTrim(__FILE__,FUNCTION,__LINE__,nurb)
#define gluBuild1DMipmapLevels(target,internalFormat,width,format,type,level,base,max,data) OpenGL_gluBuild1DMipmapLevels(__FILE__,FUNCTION,__LINE__,target,internalFormat,width,format,type,level,base,max,data)
#define gluBuild1DMipmaps(target,internalFormat,width,format,type,data) OpenGL_gluBuild1DMipmaps(__FILE__,FUNCTION,__LINE__,target,internalFormat,width,format,type,data)
#define gluBuild2DMipmapLevels(target,internalFormat,width,height,format,type,level,base,max,data) OpenGL_gluBuild2DMipmapLevels(__FILE__,FUNCTION,__LINE__,target,internalFormat,width,height,format,type,level,base,max,data)
#define gluBuild2DMipmaps(target,internalFormat,width,height,format,type,data) OpenGL_gluBuild2DMipmaps(__FILE__,FUNCTION,__LINE__,target,internalFormat,width,height,format,type,data)
#define gluBuild3DMipmapLevels(target,internalFormat,width,height,depth,format,type,level,base,max,data) OpenGL_gluBuild3DMipmapLevels(__FILE__,FUNCTION,__LINE__,target,internalFormat,width,height,depth,format,type,level,base,max,data)
#define gluBuild3DMipmaps(target,internalFormat,width,height,depth,format,type,data) OpenGL_gluBuild3DMipmaps(__FILE__,FUNCTION,__LINE__,target,internalFormat,width,height,depth,format,type,data)
#define gluCheckExtension(extName,extString) OpenGL_gluCheckExtension(__FILE__,FUNCTION,__LINE__,extName,extString)
#define gluCylinder(quad,base,top,height,slices,stacks) OpenGL_gluCylinder(__FILE__,FUNCTION,__LINE__,quad,base,top,height,slices,stacks)
#define gluDeleteNurbsRenderer(nurb) OpenGL_gluDeleteNurbsRenderer(__FILE__,FUNCTION,__LINE__,nurb)
#define gluDeleteQuadric(quad) OpenGL_gluDeleteQuadric(__FILE__,FUNCTION,__LINE__,quad)
#define gluDeleteTess(tess) OpenGL_gluDeleteTess(__FILE__,FUNCTION,__LINE__,tess)
#define gluDisk(quad,inner,outer,slices,loops) OpenGL_gluDisk(__FILE__,FUNCTION,__LINE__,quad,inner,outer,slices,loops)
#define gluEndCurve(nurb) OpenGL_gluEndCurve(__FILE__,FUNCTION,__LINE__,nurb)
#define gluEndPolygon(tess) OpenGL_gluEndPolygon(__FILE__,FUNCTION,__LINE__,tess)
#define gluEndSurface(nurb) OpenGL_gluEndSurface(__FILE__,FUNCTION,__LINE__,nurb)
#define gluEndTrim(nurb) OpenGL_gluEndTrim(__FILE__,FUNCTION,__LINE__,nurb)
#define gluErrorString(error) OpenGL_gluErrorString(__FILE__,FUNCTION,__LINE__,error)
#define gluGetNurbsProperty(nurb,property,data) OpenGL_gluGetNurbsProperty(__FILE__,FUNCTION,__LINE__,nurb,property,data)
#define gluGetString(name) OpenGL_gluGetString(__FILE__,FUNCTION,__LINE__,name)
#define gluGetTessProperty(tess,which,data) OpenGL_gluGetTessProperty(__FILE__,FUNCTION,__LINE__,tess,which,data)
#define gluLoadSamplingMatrices(nurb,model,perspective,view) OpenGL_gluLoadSamplingMatrices(__FILE__,FUNCTION,__LINE__,nurb,model,perspective,view)
#define gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ) OpenGL_gluLookAt(__FILE__,FUNCTION,__LINE__,eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ)
#define gluNewNurbsRenderer(void) OpenGL_gluNewNurbsRenderer(__FILE__,FUNCTION,__LINE__)
#define gluNewQuadric(void) OpenGL_gluNewQuadric(__FILE__,FUNCTION,__LINE__)
#define gluNewTess(void) OpenGL_gluNewTess(__FILE__,FUNCTION,__LINE__)
#define gluNextContour(tess,type) OpenGL_gluNextContour(__FILE__,FUNCTION,__LINE__,tess,type)
#define gluNurbsCallback(nurb,which,CallBackFunc) OpenGL_gluNurbsCallback(__FILE__,FUNCTION,__LINE__,nurb,which,CallBackFunc)
#define gluNurbsCallbackData(nurb,userData) OpenGL_gluNurbsCallbackData(__FILE__,FUNCTION,__LINE__,nurb,userData)
#define gluNurbsCallbackDataEXT(nurb,userData) OpenGL_gluNurbsCallbackDataEXT(__FILE__,FUNCTION,__LINE__,nurb,userData)
#define gluNurbsCurve(nurb,knotCount,knots,stride,control,order,type) OpenGL_gluNurbsCurve(__FILE__,FUNCTION,__LINE__,nurb,knotCount,knots,stride,control,order,type)
#define gluNurbsProperty(nurb,property,value) OpenGL_gluNurbsProperty(__FILE__,FUNCTION,__LINE__,nurb,property,value)
#define gluNurbsSurface(nurb,sKnotCount,sKnots,tKnotCount,tKnots,sStride,tStride,control,sOrder,tOrder,type) OpenGL_gluNurbsSurface(__FILE__,FUNCTION,__LINE__,nurb,sKnotCount,sKnots,tKnotCount,tKnots,sStride,tStride,control,sOrder,tOrder,type)
#define gluOrtho2D(left,right,bottom,top) OpenGL_gluOrtho2D(__FILE__,FUNCTION,__LINE__,left,right,bottom,top)
#define gluPartialDisk(quad,inner,outer,slices,loops,start,sweep) OpenGL_gluPartialDisk(__FILE__,FUNCTION,__LINE__,quad,inner,outer,slices,loops,start,sweep)
#define gluPerspective(fovy,aspect,zNear,zFar) OpenGL_gluPerspective(__FILE__,FUNCTION,__LINE__,fovy,aspect,zNear,zFar)
#define gluPickMatrix(x,y,delX,delY,viewport) OpenGL_gluPickMatrix(__FILE__,FUNCTION,__LINE__,x,y,delX,delY,viewport)
#define gluProject(objX,objY,objZ,model,proj,view,winX,winY,winZ) OpenGL_gluProject(__FILE__,FUNCTION,__LINE__,objX,objY,objZ,model,proj,view,winX,winY,winZ)
#define gluPwlCurve(nurb,count,data,stride,type) OpenGL_gluPwlCurve(__FILE__,FUNCTION,__LINE__,nurb,count,data,stride,type)
#define gluQuadricCallback(quad,which,CallBackFunc) OpenGL_gluQuadricCallback(__FILE__,FUNCTION,__LINE__,quad,which,CallBackFunc)
#define gluQuadricDrawStyle(quad,draw) OpenGL_gluQuadricDrawStyle(__FILE__,FUNCTION,__LINE__,quad,draw)
#define gluQuadricNormals(quad,normal) OpenGL_gluQuadricNormals(__FILE__,FUNCTION,__LINE__,quad,normal)
#define gluQuadricOrientation(quad,orientation) OpenGL_gluQuadricOrientation(__FILE__,FUNCTION,__LINE__,quad,orientation)
#define gluQuadricTexture(quad,texture) OpenGL_gluQuadricTexture(__FILE__,FUNCTION,__LINE__,quad,texture)
#define gluScaleImage(format,wIn,hIn,typeIn,dataIn,wOut,hOut,typeOut,dataOut) OpenGL_gluScaleImage(__FILE__,FUNCTION,__LINE__,format,wIn,hIn,typeIn,dataIn,wOut,hOut,typeOut,dataOut)
#define gluSphere(quad,radius,slices,stacks) OpenGL_gluSphere(__FILE__,FUNCTION,__LINE__,quad,radius,slices,stacks)
#define gluTessBeginContour(tess) OpenGL_gluTessBeginContour(__FILE__,FUNCTION,__LINE__,tess)
#define gluTessBeginPolygon(tess,data) OpenGL_gluTessBeginPolygon(__FILE__,FUNCTION,__LINE__,tess,data)
#define gluTessCallback(tess,which,CallBackFunc) OpenGL_gluTessCallback(__FILE__,FUNCTION,__LINE__,tess,which,CallBackFunc)
#define gluTessEndContour(tess) OpenGL_gluTessEndContour(__FILE__,FUNCTION,__LINE__,tess)
#define gluTessEndPolygon(tess) OpenGL_gluTessEndPolygon(__FILE__,FUNCTION,__LINE__,tess)
#define gluTessNormal(tess,valueX,valueY,valueZ) OpenGL_gluTessNormal(__FILE__,FUNCTION,__LINE__,tess,valueX,valueY,valueZ)
#define gluTessProperty(tess,which,data) OpenGL_gluTessProperty(__FILE__,FUNCTION,__LINE__,tess,which,data)
#define gluTessVertex(tess,location,data) OpenGL_gluTessVertex(__FILE__,FUNCTION,__LINE__,tess,location,data)
#define gluUnProject(winX,winY,winZ,model,proj,view,objX,objY,objZ) OpenGL_gluUnProject(__FILE__,FUNCTION,__LINE__,winX,winY,winZ,model,proj,view,objX,objY,objZ)
#define gluUnProject4(winX,winY,winZ,clipW,model,proj,view,nearVal,farVal,objX,objY,objZ,objW) OpenGL_gluUnProject4(__FILE__,FUNCTION,__LINE__,winX,winY,winZ,clipW,model,proj,view,nearVal,farVal,objX,objY,objZ,objW)

#endif

#endif
