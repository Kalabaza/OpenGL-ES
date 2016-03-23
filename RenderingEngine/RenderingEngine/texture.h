#pragma once

// STD
#include <string>
#include <vector>
#include <memory>

// OpenGL
#include <GLES2/gl2.h>

// Engine
#include "logger.h"
#include "shader.h"

namespace RenderingEngine
{
    using std::vector;
    using std::string;
    using std::unique_ptr;

    class Texture
    {
    private:
        GLint width, height, numComponents;
        vector<unsigned char> pixels;

        unsigned char * data;

        GLuint textureObject;

        unique_ptr<Shader> &shader;
    public:
        Texture(const string&, unique_ptr<Shader>&);
        ~Texture();

        void draw();
    };
}
