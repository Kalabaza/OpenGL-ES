#pragma once

#include <GLES2/gl2.h>
#include <EGL/egl.h>

// Macros
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl

// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

typedef struct
{
    // User data
    void* userData;

    /// Window width
    GLint width;

    /// Window height
    GLint height;

    /// Window handle
    EGLNativeWindowType hWnd;

    /// EGL display
    EGLDisplay eglDisplay;

    /// EGL context
    EGLContext eglContext;

    /// EGL surface
    EGLSurface eglSurface;

    /// Callbacks
    void (ESCALLBACK *drawFunc) (void*);
    void (ESCALLBACK *keyFunc) (void*, unsigned char, int, int);
    void (ESCALLBACK *updateFunc) (void*, float deltaTime);
} ESContext;


// Initializes OpenGL ES context, must be called before any other functions
void ESUTIL_API esInitContext(ESContext *esContext);

// Create a window with the specified characteristics
GLboolean ESUTIL_API esCreateWindow(ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags);

void ESUTIL_API esMainLoop(ESContext *esContext);

void WinLoop(ESContext *esContext);

void ESUTIL_API esRegisterDrawFunc(ESContext *esContext, void (ESCALLBACK *drawFunc) (ESContext*));

// Create a Win32 window
GLboolean WinCreate(ESContext *esContext, const char *title);