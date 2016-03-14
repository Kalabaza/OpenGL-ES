#include <iostream>
#include "Win32.h"

// Creating of a window in Win32
int WinCreate(ESContext *esContext, const char *title)
{
    // Get a handle to the running application
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASS wndclass = { 0 };
    wndclass.style = CS_OWNDC;
    wndclass.lpfnWndProc = (WNDPROC)ESWinProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, "IDI_ICON1");
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = "opengles2.0";

    if (RegisterClass(&wndclass) == FALSE)
    {
        std::cerr << "Could not register the class for the Win32 window." << std::endl;
        return FALSE;
    }

    // Change the style of the window just created
    DWORD wStyle = WS_BORDER | WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_VISIBLE;

    // Adjust the window rectangle so that the client area has the correct number of pixels
    RECT     windowRect;
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = esContext->width;
    windowRect.bottom = esContext->height;

    // Adjust the window size and style
    AdjustWindowRect(&windowRect, wStyle, FALSE);

    esContext->hWnd = CreateWindow("opengles2.0", title, wStyle, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
                                   nullptr, nullptr, hInstance, nullptr);

    if (esContext->hWnd == nullptr)
    {
        std::cerr << "Error at the creation of the Win32 window." << std::endl;
        return FALSE;
    }

    // Set the ESContext* to the GWLP_USERDATA so that it is available to the ESWinProc
    SetWindowLongPtr(esContext->hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)esContext);

    // Show the window on screen
    ShowWindow(esContext->hWnd, TRUE);

    return TRUE;
}

// Handle the messages sent to the application
LRESULT CALLBACK ESWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT  lRet = 1;

    switch (uMsg)
    {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        // Quit the application
        PostQuitMessage(0);
        break;
    // Handle the redisplay of the screen
    case WM_PAINT:
        {
            // Get the user data (context) and trigger a re-draw on screen
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (esContext != nullptr && esContext->drawFunc != nullptr)
                esContext->drawFunc(esContext);

            // Validates the client area
            ValidateRect(esContext->hWnd, nullptr);
        }
        break;
    // Handle the keyboard pressed keys
    case WM_CHAR:
        {
            // Get the position of the cursor in screen coordinates
            POINT point;
            GetCursorPos(&point);

            // Get the user data (context) and send the key information
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (esContext != nullptr && esContext->keyFunc != nullptr)
                esContext->keyFunc(esContext, (unsigned char)wParam, (int)point.x, (int)point.y);
        }
        break;
    // Use the default message processing for the messages not handled by the application
    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

// Main loop of the application
void WinLoop(ESContext *esContext)
{
    MSG msg = { 0 };
    int done = 0;
    DWORD lastTime = GetTickCount();

    while (!done)
    {
        int gotMsg = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        DWORD curTime = GetTickCount();
        float deltaTime = (float)(curTime - lastTime) / 1000.0f;
        lastTime = curTime;

        if (gotMsg != FALSE)
        {
            // Message to quit the application
            if (msg.message == WM_QUIT)
            {
                done = 1;
            }
            else
            {
                // Translate the message and dispatch it
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
            // Trigger a repaint of the screen
            SendMessage(esContext->hWnd, WM_PAINT, 0, 0);

        // Call update function if registered
        if (esContext->updateFunc != nullptr)
            esContext->updateFunc(esContext, deltaTime);
    }
}
