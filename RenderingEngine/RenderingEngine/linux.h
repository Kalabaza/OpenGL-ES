#pragma once

#include <unistd.h>

// X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// OpenGL
#include <GLES2/gl2.h>
#include <EGL/egl.h>

// Render
#include "logger.h"

namespace RenderingEngine
{
    bool CreateX11Window(EGLDisplay, Display*, EGLConfig&, void*);

    void LinuxLoop(void*);
}
