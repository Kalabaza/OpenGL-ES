#include "linux.h"
#include "openGLES.h"

namespace RenderingEngine
{
    bool CreateX11Window(EGLDisplay egldisplay, Display *display, EGLConfig &eglConfig, void *esContext)
    {
        XSetWindowAttributes windowAttributes;
        XSizeHints sizeHints;
        XVisualInfo visualInfoTemplate;

        unsigned long mask;
        long screen;

        int visualID, numberOfVisuals;

        ((ESContext*)esContext)->display = display;
        screen = DefaultScreen(display);

        eglGetConfigAttrib(egldisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &visualID);
        visualInfoTemplate.visualid = visualID;
        XVisualInfo *visual = XGetVisualInfo(display, VisualIDMask, &visualInfoTemplate, &numberOfVisuals);

        if (visual == NULL)
        {
            Log << Error << "Couldn't get X visual info" << endl;
            return false;
        }

        Colormap colormap = XCreateColormap(display, RootWindow(display, screen), visual->visual, AllocNone);

        windowAttributes.colormap = colormap;
        windowAttributes.background_pixel = 0xFFFFFFFF;
        windowAttributes.border_pixel = 0;
        windowAttributes.event_mask = StructureNotifyMask | ExposureMask;

        mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;
        // TODO fix the width and height
        ((ESContext*)esContext)->hWnd = XCreateWindow(display, RootWindow(display, screen), 0, 0, 1024, 768,
            0, visual->depth, InputOutput, visual->visual, mask, &windowAttributes);
        sizeHints.flags = USPosition;
        sizeHints.x = 10;
        sizeHints.y = 10;
        // TODO fix the title of the window
        XSetStandardProperties(display, ((ESContext*)esContext)->hWnd, "Title", "", None, 0, 0, &sizeHints);
        XMapWindow(display, ((ESContext*)esContext)->hWnd);
        XSetWMColormapWindows(display, ((ESContext*)esContext)->hWnd, &((ESContext*)esContext)->hWnd, 1);
        XFlush(display);

        XSelectInput(display, ((ESContext*)esContext)->hWnd, KeyPressMask | ExposureMask | EnterWindowMask
                     | LeaveWindowMask | PointerMotionMask | VisibilityChangeMask | ButtonPressMask
                     | ButtonReleaseMask | StructureNotifyMask);

        return true;
    }

    void LinuxLoop(void *esContext)
    {
        XEvent event;
        KeySym key;
        char text[255];

        while(true)
        {
            while (XPending(((ESContext*)esContext)->display) > 0) 
            {
                XNextEvent(((ESContext*)esContext)->display, &event);

                switch(event.type)
                {
                    // Key press
                    case KeyPress:
                        if (XLookupString(&event.xkey,text,255,&key,0)==1)
                        {
                            ((ESContext*)esContext)->keyFunc(esContext, (unsigned char) text[0], event.xbutton.x, event.xbutton.y);
                            ((ESContext*)esContext)->updateFunc((ESContext*)esContext, 0.0);
                        }
                        break;
                    case Expose:
                        ((ESContext*)esContext)->updateFunc((ESContext*)esContext, 0.0);
                        break;
                    // Mouse click
                    case ButtonPress:
                        break;
                }
            }
            ((ESContext*)esContext)->drawFunc((ESContext*)esContext);
        }
    }
}
