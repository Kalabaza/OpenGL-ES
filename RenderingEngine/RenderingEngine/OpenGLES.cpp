#include <iostream>
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

// Create the OpenGL Es surface area and the window
GLuint esCreateWindow(ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags)
{
    // Check that the received context pointer is valid
    if (esContext == nullptr)
    {
        std::cerr << "Invalid pointer received for the context." << std::endl;
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
        EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA)       ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH)       ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL)     ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
        EGL_NONE
    };

    // Copy the window width and height into the context
    esContext->width  = width;
    esContext->height = height;

    // Create a Win32 window for the rendering area
    if (WinCreate(esContext, title) == FALSE)
    {
        std::cerr << "Creation of the Win32 window failed." << std::endl;
        return EGL_FALSE;
    }

    // Create the EGL context and attach the rendering area to the on screen window
    if (CreateEGLContext(esContext->hWnd, &esContext->eglDisplay, &esContext->eglContext, &esContext->eglSurface, attribList) == EGL_FALSE)
    {
        std::cerr << "Error on the method CreateEGLContext." << std::endl;
        return EGL_FALSE;
    }

    return EGL_TRUE;
}

// Create the display connection, rendering area and context
EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd, EGLDisplay* eglDisplay, EGLContext* eglContext, EGLSurface* eglSurface, EGLint attribList[])
{
    // Get the EGL display connection
    EGLDisplay display = eglGetDisplay(GetDC(hWnd));
    if (display == EGL_NO_DISPLAY)
    {
        std::cerr << "Unable to open connection to local windowing system." << std::endl;
        return EGL_FALSE;
    }

    // Initialize the EGL display connection
    EGLint majorVersion;
    EGLint minorVersion;
    if (eglInitialize(display, &majorVersion, &minorVersion) == EGL_FALSE)
    {
        std::cerr << "Unable to initialize EGL." << std::endl;
        return EGL_FALSE;
    }

    // Get the supported surface configurations from EGL
    EGLint numConfigs;
    if (eglGetConfigs(display, nullptr, 0, &numConfigs) == EGL_FALSE)
    {
        std::cerr << "Unable to get the number of available configurations of EGL." << std::endl;
        return EGL_FALSE;
    }

    // Choose the best configuration for the specified attributes
    EGLConfig config;
    if (eglChooseConfig(display, attribList, &config, 1, &numConfigs) == EGL_FALSE)
    {
        std::cerr << "Unable to find a matching configuration with EGL." << std::endl;
        return EGL_FALSE;
    }

    // Create a on screen rendering area with EGL
    EGLSurface surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, nullptr);
    if (surface == EGL_NO_SURFACE)
    {
        std::cerr << "Unable to create a rendering surface with EGL." << std::endl;
        return EGL_FALSE;
    }

    // Attributes for the OpenGL ES context
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

    // Create a rendering context with EGL
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        std::cerr << "Unable to create a rendering context with EGL." << std::endl;
        return EGL_FALSE;
    }

    // Attach a the context to the rendering surface with EGL
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        std::cerr << "Unable to attach the EGL context to the rendering surface." << std::endl;
        return EGL_FALSE;
    }

    *eglDisplay = display;
    *eglSurface = surface;
    *eglContext = context;

    return EGL_TRUE;
}

// Register the draw function in OpenGL ES
void esRegisterDrawFunc(ESContext *esContext, void (*drawFunc) (ESContext*))
{
    esContext->drawFunc = reinterpret_cast<void(__cdecl *)(void *)>(drawFunc);
}

// Register the keyboard function in OpenGL ES
void esRegisterKeyFunc(ESContext *esContext, void(*keyboardFunc) (ESContext*, unsigned char, int, int))
{
    esContext->keyFunc = reinterpret_cast<void(__cdecl *)(void *, unsigned char, int, int)>(keyboardFunc);
}

// Main loop of the application
void esMainLoop(ESContext *esContext)
{
    WinLoop(esContext);
}

// Method to load a shader into the OpenGL ES application
GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;

    // Create the shader object of the specified type
    shader = glCreateShader(type);

    // Check if the creation of the shader was correct
    if (shader == GL_FALSE)
        return GL_FALSE;

    // Load the shader source code
    glShaderSource(shader, 1, &shaderSrc, nullptr);

    // Compile the shader
    glCompileShader(shader);

    // Check the compilation status of the shader
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        // Get information about the error
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);

            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            std::cerr << "Error compiling shader: " << infoLog << std::endl;

            free(infoLog);
        }

        glDeleteShader(shader);
        return GL_FALSE;
    }

    // Return the shader number
    return shader;
}
