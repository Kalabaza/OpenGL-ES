#include "OpenGLES.h"
#include "Win32.h"

// Initializes the OpenGL ES context variable, must be called before any other functions
void esInitContext(ESContext *esContext)
{
    if (esContext != nullptr)
    {
        memset(esContext, 0, sizeof(ESContext));
    }
}

GLboolean esCreateWindow(ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags)
{
    EGLint attribList[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
        EGL_NONE
    };

    if (esContext == nullptr)
    {
        return GL_FALSE;
    }

    esContext->width = width;
    esContext->height = height;

    if (!WinCreate(esContext, title))
    {
        return GL_FALSE;
    }


    if (!CreateEGLContext(esContext->hWnd,
        &esContext->eglDisplay,
        &esContext->eglContext,
        &esContext->eglSurface,
        attribList))
    {
        return GL_FALSE;
    }


    return GL_TRUE;
}

EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
    EGLContext* eglContext, EGLSurface* eglSurface,
    EGLint attribList[])
{
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

    // Get Display
    display = eglGetDisplay(GetDC(hWnd));
    if (display == EGL_NO_DISPLAY)
    {
        return EGL_FALSE;
    }

    // Initialize EGL
    if (!eglInitialize(display, &majorVersion, &minorVersion))
    {
        return EGL_FALSE;
    }

    // Get configs
    if (!eglGetConfigs(display, nullptr, 0, &numConfigs))
    {
        return EGL_FALSE;
    }

    // Choose config
    if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
    {
        return EGL_FALSE;
    }

    // Create a surface
    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, nullptr);
    if (surface == EGL_NO_SURFACE)
    {
        return EGL_FALSE;
    }

    // Create a GL context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        return EGL_FALSE;
    }

    // Make the context current
    if (!eglMakeCurrent(display, surface, surface, context))
    {
        return EGL_FALSE;
    }

    *eglDisplay = display;
    *eglSurface = surface;
    *eglContext = context;
    return EGL_TRUE;
}

void esRegisterDrawFunc(ESContext *esContext, void (*drawFunc) (ESContext*))
{
    esContext->drawFunc = reinterpret_cast<void(__cdecl *)(void *)>(drawFunc);
}

void esMainLoop(ESContext *esContext)
{
    WinLoop(esContext);
}
