#include "openGLES.h"
#if defined(_WIN32)
#include "win32.h"
#elif defined(__linux__)
#include <cstring>
#endif

namespace RenderingEngine
{
    // Initializes the OpenGL ES context variable, must be called before any other functions
    void esInitContext(ESContext *esContext)
    {
        Log << Function << endl;
        if (esContext != nullptr)
        {
            memset(esContext, 0, sizeof(ESContext));
        }
    }

    // Create the OpenGL Es surface area and the window
    GLuint esCreateWindow(ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags)
    {
        Log << Function << endl;
        // Check that the received context pointer is valid
        if (esContext == nullptr)
        {
            Log << Error << "Invalid pointer received for the context." << endl;
            return EGL_FALSE;
        }

        // Attributes of the OpenGL ES 
        EGLint attribList[] =
        {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            // Choose the RGB 565 format
            EGL_RED_SIZE,       5,
            EGL_GREEN_SIZE,     6,
            EGL_BLUE_SIZE,      5,
            EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
            EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
            EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
            EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
            EGL_NONE
        };

        // Copy the window width and height into the context
        esContext->width = width;
        esContext->height = height;

#if defined(_WIN32)
        // Create a Win32 window for the rendering area
        if (WinCreate(esContext, title) == FALSE)
        {
            Log << Error << "Creation of the Win32 window failed." << endl;
            return EGL_FALSE;
        }
#endif

        // Create the EGL context and attach the rendering area to the on screen window
        if (CreateEGLContext(esContext, attribList) == EGL_FALSE)
        {
            Log << Error << "Error on the method CreateEGLContext." << endl;
            return EGL_FALSE;
        }

        return EGL_TRUE;
    }

    GLuint esDestroyWindow(ESContext *esContext)
    {
        eglBindAPI(EGL_OPENGL_ES_API);
        // Disable the rendering surface and context of OpenGL ES
        eglMakeCurrent(esContext->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, esContext->eglContext);
        eglMakeCurrent(esContext->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        // Destroy the context of OpenGL ES
        if (eglDestroyContext(esContext->eglDisplay, esContext->eglContext) == EGL_FALSE)
        {
            Log << Error << "Unable to destroy the context with EGL." << endl;
            return EGL_FALSE;
        }
        // Destroy the rendering surface
        if (eglDestroySurface(esContext->eglDisplay, esContext->eglSurface) == EGL_FALSE)
        {
            Log << Error << "Unable to destroy the surface with EGL." << endl;
            return EGL_FALSE;
        }
        // Release EGL display connection
        if (eglTerminate(esContext->eglDisplay) == EGL_FALSE)
        {
            Log << Error << "Unable to terminate the display connection with EGL." << endl;
            return EGL_FALSE;
        }

#if defined(_WIN32)
        // Destroy the Win32 window
        if (WinDestroy(esContext) == FALSE)
        {
            Log << Error << "Destruction of the Win32 window failed." << endl;
            return EGL_FALSE;
        }
#endif

        return EGL_TRUE;
    }

    // Create the display connection, rendering area and context
    EGLBoolean CreateEGLContext(ESContext *esContext, EGLint attribList[])
    {
        Log << Function << endl;
        // Get the EGL display connection
#if defined(_WIN32)
        EGLDisplay display = eglGetDisplay(GetDC(esContext->hWnd));
#elif defined(__linux__)
        Display *hdisplay = XOpenDisplay(NULL);
        EGLDisplay display = eglGetDisplay(hdisplay);
#endif
        if (display == EGL_NO_DISPLAY)
        {
            Log << Error << "Unable to open connection to local windowing system." << endl;
            return EGL_FALSE;
        }

        // Initialize the EGL display connection
        EGLint majorVersion;
        EGLint minorVersion;
        if (eglInitialize(display, &majorVersion, &minorVersion) == EGL_FALSE)
        {
            Log << Error << "Unable to initialize EGL." << endl;
            return EGL_FALSE;
        }

        // Get the supported surface configurations from EGL
        EGLint numConfigs;
        if (eglGetConfigs(display, nullptr, 0, &numConfigs) == EGL_FALSE)
        {
            Log << Error << "Unable to get the number of available configurations of EGL." << endl;
            return EGL_FALSE;
        }

        // Choose the best configuration for the specified attributes
        EGLConfig config;
        if (eglChooseConfig(display, attribList, &config, 1, &numConfigs) == EGL_FALSE)
        {
            Log << Error << "Unable to find a matching configuration with EGL." << endl;
            return EGL_FALSE;
        }

#if defined(__linux__)
        CreateX11Window(display, hdisplay, config, esContext);
#endif

        // Create a on screen rendering area with EGL
        EGLSurface surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)esContext->hWnd, nullptr);
        if (surface == EGL_NO_SURFACE)
        {
            Log << Error << "Unable to create a rendering surface with EGL." << endl;
            return EGL_FALSE;
        }

        // Attributes for the OpenGL ES context
        EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

        // Create a rendering context with EGL
        EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
        if (context == EGL_NO_CONTEXT)
        {
            Log << Error << "Unable to create a rendering context with EGL." << endl;
            return EGL_FALSE;
        }

        // Attach a the context to the rendering surface with EGL
        if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
        {
            Log << Error << "Unable to attach the EGL context to the rendering surface." << endl;
            return EGL_FALSE;
        }

        esContext->eglDisplay = display;
        esContext->eglSurface = surface;
        esContext->eglContext = context;

        return EGL_TRUE;
    }

    // Register the draw function in OpenGL ES
    void esRegisterDrawFunc(ESContext *esContext, void(*drawFunc)(ESContext*))
    {
        Log << Function << endl;
        esContext->drawFunc = reinterpret_cast<void(__cdecl *)(void *)>(drawFunc);
    }

    // Register the keyboard function in OpenGL ES
    void esRegisterKeyFunc(ESContext *esContext, void(*keyboardFunc)(ESContext*, unsigned char, int, int))
    {
        Log << Function << endl;
        esContext->keyFunc = reinterpret_cast<void(__cdecl *)(void *, unsigned char, int, int)>(keyboardFunc);
    }

    // Register the update function in OpenGL ES
    void esRegisterUpdateFunc(ESContext *esContext, void(*updateFunc)(ESContext*, float))
    {
        esContext->updateFunc = reinterpret_cast<void(__cdecl *)(void *, float)>(updateFunc);
    }

    // Main loop of the application
    void esMainLoop(ESContext *esContext)
    {
#if defined(_WIN32)
        WinLoop(esContext);
#elif defined(__linux__)
        LinuxLoop(esContext);
#endif
    }
}
