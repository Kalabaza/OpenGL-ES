#pragma once

#include <iostream>
#include "OpenGLES.h"

GLuint LoadShader(GLenum type, const char *shaderSrc);

int Init(ESContext *esContext);
