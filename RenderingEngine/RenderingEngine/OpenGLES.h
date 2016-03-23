#pragma once

// OpenGL
#include <GLES2/gl2.h>
#include <EGL/egl.h>

// Engine
#include "logger.h"

namespace RenderingEngine
{
    // esCreateWindow flags
    const int ES_WINDOW_RGB = 0;  // RGB based color buffer
    const int ES_WINDOW_ALPHA = 1;  // Allocate an alpha color buffer
    const int ES_WINDOW_DEPTH = 2;  // Allocate a depth buffer
    const int ES_WINDOW_STENCIL = 4;  // Allocate a stencil buffer
    const int ES_WINDOW_MULTISAMPLE = 8;  // Allocate a multi-sample buffer

    typedef struct
    {
        // User data pointer
        GLuint programID;

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
        void(*drawFunc)   (void*);
        void(*keyFunc)    (void*, unsigned char, int, int);
        void(*updateFunc) (void*, float);
    } ESContext;

    // Initializes the OpenGL ES context variable, must be called before any other functions
    void esInitContext(ESContext*);

    // Create the OpenGL Es surface area and the window
    GLuint esCreateWindow(ESContext*, const char*, GLint, GLint, GLuint);

    // Create the display connection, rendering area and context
    EGLBoolean CreateEGLContext(EGLNativeWindowType, EGLDisplay*, EGLContext*, EGLSurface*, EGLint[]);

    // Register the draw function in OpenGL ES
    void esRegisterDrawFunc(ESContext*, void(*drawFunc)(ESContext*));

    // Register the keyboard function in OpenGL ES
    void esRegisterKeyFunc(ESContext*, void(*keyboardFunc)(ESContext*, unsigned char, int, int));

    // Register the update function in OpenGL ES
    void esRegisterUpdateFunc(ESContext*, void(*updateFunc)(ESContext*, float));

    // Main loop of the application
    void esMainLoop(ESContext*);
}
