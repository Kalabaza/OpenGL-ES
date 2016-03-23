#include "openGLES.h"
#include "win32.h"

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

        // Create a Win32 window for the rendering area
        if (WinCreate(esContext, title) == FALSE)
        {
            Log << Error << "Creation of the Win32 window failed." << endl;
            return EGL_FALSE;
        }

        // Create the EGL context and attach the rendering area to the on screen window
        if (CreateEGLContext(esContext->hWnd, &esContext->eglDisplay, &esContext->eglContext, &esContext->eglSurface, attribList) == EGL_FALSE)
        {
            Log << Error << "Error on the method CreateEGLContext." << endl;
            return EGL_FALSE;
        }

        return EGL_TRUE;
    }

    // Create the display connection, rendering area and context
    EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd, EGLDisplay* eglDisplay, EGLContext* eglContext, EGLSurface* eglSurface, EGLint attribList[])
    {
        Log << Function << endl;
        // Get the EGL display connection
        EGLDisplay display = eglGetDisplay(GetDC(hWnd));
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

        // Create a on screen rendering area with EGL
        EGLSurface surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, nullptr);
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

        *eglDisplay = display;
        *eglSurface = surface;
        *eglContext = context;

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
        WinLoop(esContext);
    }
}
