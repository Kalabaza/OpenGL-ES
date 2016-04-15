#pragma once

// Render
#include "logger.h"
#include "openGLES.h"

namespace RenderingEngine
{
    // Creating of a window in Win32
    int WinCreate(ESContext*, const char*);

    // Destroying a window in Win32
    int WinDestroy(ESContext*);

    // Handle the messages sent to the application
    LRESULT WINAPI ESWinProc(HWND, UINT, WPARAM, LPARAM);

    // Main loop of the application
    void WinLoop(ESContext*);
}
