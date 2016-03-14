#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "OpenGLES.h"

// Creating of a window in Win32
int WinCreate(ESContext *esContext, const char *title);

// Handle the messages sent to the application
LRESULT WINAPI ESWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Main loop of the application
void WinLoop(ESContext *esContext);
