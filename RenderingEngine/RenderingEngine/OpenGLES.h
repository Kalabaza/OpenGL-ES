#pragma once

#include <GLES2/gl2.h>
#include <EGL/egl.h>

using namespace std;

// esCreateWindow flags
#define ES_WINDOW_RGB         0     // RGB color buffer
#define ES_WINDOW_ALPHA       1     // ALPHA color buffer
#define ES_WINDOW_DEPTH       2     // Depth buffer
#define ES_WINDOW_STENCIL     4     // Stencil buffer
#define ES_WINDOW_MULTISAMPLE 8     // Multi-sample buffer

typedef struct
{
    // Handle to a program object created with OpenGL
    GLuint programObject;

} UserData;

typedef struct
{
    // User data pointer
    UserData* userData;

    // Window width
    GLint width;

    // Window height
    GLint height;

    // Window handle
    EGLNativeWindowType hWnd;

    // EGL display
    EGLDisplay eglDisplay;

    // EGL context
    EGLContext eglContext;

    // EGL surface
    EGLSurface eglSurface;

    // Callbacks for the Draw, Keyboard and Update methods
    void (*drawFunc)   (void*);
    void (*keyFunc)    (void*, unsigned char, int, int);
    void (*updateFunc) (void*, float deltaTime);
} ESContext;

// Initializes the OpenGL ES context variable, must be called before any other functions
void esInitContext(ESContext *esContext);

// Create the OpenGL Es window
GLboolean esCreateWindow(ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags);

void esRegisterDrawFunc(ESContext *esContext, void (*drawFunc) (ESContext*));

EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
    EGLContext* eglContext, EGLSurface* eglSurface,
    EGLint attribList[]);

void esMainLoop(ESContext *esContext);
