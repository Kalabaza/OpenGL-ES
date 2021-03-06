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
    using std::shared_ptr;

    enum TextureType
    {
        SingleTexture,
        CubeMap
    };

    const unsigned int NumCubeFaces = 6;

    class Texture
    {
    private:
        GLint width, height, numComponents;
        vector<vector<unsigned char>> pixels;

        unsigned char * data;

        GLuint textureObject;

        shared_ptr<Shader> &shader;

        void GenTextureObject(TextureType);
    public:
        // Disable the default constructor
        Texture() = delete;

#if defined(__ANDROID__)
        Texture(AAssetManager**, const string&, shared_ptr<Shader>&);
        Texture(AAssetManager**, const vector<string>&, shared_ptr<Shader>&);
#else
        Texture(const string&, shared_ptr<Shader>&);
        Texture(const vector<string>&, shared_ptr<Shader>&);
#endif
        ~Texture();

        void Draw(TextureType);

        GLint GetWidth() { return width; }
        GLint GetHeight() { return height; }

        unsigned char *TextureData() { return pixels[0].data(); }
    };
}
