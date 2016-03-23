#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace RenderingEngine
{
    Texture::Texture(const string &textureFileName, unique_ptr<Shader> &shader) : width{ 0 }, height{ 0 }, data{ nullptr }, shader{ shader }
    {
        Log << Function << endl;
        data = stbi_load(textureFileName.c_str(), reinterpret_cast<int*>(&width), reinterpret_cast<int*>(&height), &numComponents, 3);

        if (data == nullptr)
        {
            Log << Error << "Texture format not supported: " << textureFileName << endl;
            exit(1);
        }

        // Copy the data to the vector that is used on the other textures
        pixels = vector<unsigned char>(data, data + (width * height * 3));

        // Release the original memory of the pointer
        stbi_image_free(data);

        Log << Debug << "Texture file \"" << textureFileName << "\" loaded to memory." << endl;

        glGenTextures(1, &textureObject);
        glBindTexture(GL_TEXTURE_2D, textureObject);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Log << Info << "Texture " << textureFileName << " loaded correctly." << endl;
    }

    Texture::~Texture()
    {
        Log << Function << endl;
        glDeleteTextures(1, &textureObject);
    }

    void Texture::draw()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureObject);
        for (auto &uniform : shader->getUniforms())
        {
            glUniform1i(uniform->getVariableIndex(), 0);
        }
    }
}
