#pragma once

// STD
#include <memory>

// Engine
#include "logger.h"
#include "openGLES.h"
#include "scenemanager.h"

namespace RenderingEngine
{
    using std::unique_ptr;
    using std::make_unique;

    // Size of the rendering window
    const int Width = 1024;
    const int Height = 768;

    unique_ptr<SceneManager> sceneManager;

    GLuint tertureBuffer;

    // Method to initialize the OpenGL ES related information
    GLuint Init(ESContext *esContext);

    // Method used to draw on screen
    void Draw(ESContext *esContext);

    // Method handle keyboard pressed keys
    void Key(ESContext *esContext, unsigned char, int, int);
}
