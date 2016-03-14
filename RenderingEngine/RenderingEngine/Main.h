#pragma once

#include <iostream>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "OpenGLES.h"

const int Width  = 800;
const int Height = 600;

// Method to initialize the OpenGL ES related information
GLuint Init(ESContext *esContext);

// Method used to draw on screen
void Draw(ESContext *esContext);

// Method handle keyboard pressed keys
void Key(ESContext *esContext, unsigned char, int, int);
