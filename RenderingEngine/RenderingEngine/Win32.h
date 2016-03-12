#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "OpenGLES.h"

GLboolean WinCreate(ESContext *esContext, const char *title);

LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void WinLoop(ESContext *esContext);
