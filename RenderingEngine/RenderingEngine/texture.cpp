#include "texture.h"

// Enable the STB_IMAGE library to load images from disk
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace RenderingEngine
{
#if defined(__ANDROID__)
    Texture::Texture(AAssetManager **mgr, const string &textureFileName, shared_ptr<Shader> &shader) : width{ 0 }, height{ 0 }, data{ nullptr }, shader( shader )
#else
    Texture::Texture(const string &textureFileName, shared_ptr<Shader> &shader) : width{ 0 }, height{ 0 }, data{ nullptr }, shader( shader )
#endif
    {
        Log << Function << endl;

        GenTextureObject(SingleTexture);

        // Load the input image
#if defined(__ANDROID__)
        AAsset* pFile = AAssetManager_open(*mgr, textureFileName.c_str(), AASSET_MODE_UNKNOWN);
        if (pFile)
        {
            // Get the file size
            size_t fileSize = AAsset_getLength(pFile);
            // Read data from the file
            unsigned char* pData = (unsigned char*)calloc(fileSize + 1, sizeof(unsigned char));
            AAsset_read(pFile, pData, fileSize);
            // fix the string to be zero-terminated
            pData[fileSize] = 0;
            data = stbi_load_from_memory(pData, fileSize, reinterpret_cast<int*>(&width), reinterpret_cast<int*>(&height), &numComponents, 3);
            AAsset_close(pFile);
            free(pData);
        }
#else
        data = stbi_load(textureFileName.c_str(), reinterpret_cast<int*>(&width), reinterpret_cast<int*>(&height), &numComponents, 3);
#endif

        if (data == nullptr)
        {
            Log << Error << "Texture format not supported: " << textureFileName << endl;
            terminate();
        }

        // Copy the data to the vector that is used on the other textures
        pixels.resize(1);
        pixels[0] = vector<unsigned char>(data, data + (width * height * 3));

        // Release the original memory of the pointer
        stbi_image_free(data);

        // Send the pixel data to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels[0].data());

        Log << Info << "Texture " << textureFileName << " loaded correctly." << endl;
    }

#if defined(__ANDROID__)
    Texture::Texture(AAssetManager **mgr, const vector<string> &texturesFileNames, shared_ptr<Shader> &shader) : width{ 0 }, height{ 0 }, data{ nullptr }, shader( shader )
#else
    Texture::Texture(const vector<string> &texturesFileNames, shared_ptr<Shader> &shader) : width{ 0 }, height{ 0 }, data{ nullptr }, shader( shader )
#endif
    {
        Log << Function << endl;

        GenTextureObject(CubeMap);

        // In a cube map at least 6 images are needed to create it
        pixels.resize(NumCubeFaces);
        for (size_t index = 0; index < NumCubeFaces; ++index)
        {
            // Load the input image
#if defined(__ANDROID__)
            AAsset* pFile = AAssetManager_open(*mgr, texturesFileNames[index].c_str(), AASSET_MODE_UNKNOWN);
            if (pFile)
            {
                // Get the file size
                size_t fileSize = AAsset_getLength(pFile);
                // Read data from the file
                unsigned char* pData = (unsigned char*)calloc(fileSize + 1, sizeof(unsigned char));
                AAsset_read(pFile, pData, fileSize);
                // fix the string to be zero-terminated
                pData[fileSize] = 0;
                data = stbi_load_from_memory(pData, fileSize, reinterpret_cast<int*>(&width), reinterpret_cast<int*>(&height), &numComponents, 3);
                AAsset_close(pFile);
                free(pData);
            }
#else
            data = stbi_load(texturesFileNames[index].c_str(), reinterpret_cast<int*>(&width), reinterpret_cast<int*>(&height), &numComponents, 3);
#endif

            if (data == nullptr)
            {
                Log << Error << "Texture format not supported: " << texturesFileNames[index] << endl;
                terminate();
            }

            // Copy the data to the vector that is used on the other textures
            pixels[index].resize(1);
            pixels[index] = vector<unsigned char>(data, data + (width * height * 3));

            // Release the original memory of the pointer
            stbi_image_free(data);

            // Send the pixel data to OpenGL
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels[index].data());

            // Remove the local copy of the pixels since they are already on OpenGL
            pixels[index].clear();
        }

        Log << Debug << "Textures for the cube map loaded correctly." << endl;
    }

    Texture::~Texture()
    {
        Log << Function << endl;
        // Delete the texture from OpenGL
        glDeleteTextures(1, &textureObject);
    }

    void Texture::GenTextureObject(TextureType textureType)
    {
        // Generate the textureID and activate the texture number 0
        glGenTextures(1, &textureObject);
        glActiveTexture(GL_TEXTURE0);
        // Get the target either a normal texture or a skybox (cubemap)
        unsigned int target = (textureType == SingleTexture) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
        glBindTexture(target, textureObject);

        // Set some parameters for min/mag filtering and how the texture is going to
        // behave while being applied to an object
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void Texture::Draw(TextureType textureType)
    {
        // Bind the texture
        glBindTexture((textureType == SingleTexture) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, textureObject);
        for (auto &uniform : shader->getUniforms())
        {
            // Send the texture values to the shaders
            if(uniform->getVariableIndex() != -1)
                glUniform1i(uniform->getVariableIndex(), 0);
        }
    }
}
