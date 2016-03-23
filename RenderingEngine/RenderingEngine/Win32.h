#pragma once

// Render
#include "logger.h"
#include "openGLES.h"

namespace RenderingEngine
{
    // Creating of a window in Win32
    int WinCreate(ESContext *esContext, const char *title);

    // Handle the messages sent to the application
    LRESULT WINAPI ESWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Main loop of the application
    void WinLoop(ESContext *esContext);
}
