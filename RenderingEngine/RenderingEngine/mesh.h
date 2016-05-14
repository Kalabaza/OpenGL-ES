#pragma once

// STD
#include <vector>
#include <memory>

// OpenGL
#include <GLES2/gl2.h>

// Engine
#include "logger.h"
#include "object.h"
#include "shader.h"
#include "texture.h"

namespace RenderingEngine
{
    using std::vector;
    using std::unique_ptr;
    using std::make_unique;
    using std::shared_ptr;
    using std::string;

    class Mesh
    {
    private:
        // Pointer to the object that will be loaded from disk
        unique_ptr<Object> obj;

        // Buffer objects handle for the vertices and indexes
        GLuint vertexBuffer;
        GLuint indexBuffer;

        // Pointer to the shader that will be used to display this object
        shared_ptr<Shader> shader;
    public:
        // Disable the default constructor
        Mesh() = delete;
        // Overloaded constructor that receives a shader object since its needed to set the attributes
        // also it can receive the texture to calculate the heightmap for the plane
#if defined (__ANDROID__)
        Mesh(AAssetManager**, const string&, shared_ptr<Shader>&, unique_ptr<Texture>* = nullptr);
#else
        Mesh(const string&, shared_ptr<Shader>&, unique_ptr<Texture>* = nullptr);
#endif
        // Deep copy constructor
        Mesh(const Mesh &original) : vertexBuffer{ original.vertexBuffer }, indexBuffer{ original.indexBuffer }, shader{ original.shader }
        {
            obj = make_unique<Object>(*original.obj);
        }
        // Destructor to release resources used by the mesh
        ~Mesh();

        // Method to Draw the object on screen
        void Draw();
        // Use the shader program to display the object
        void UseProgram() { shader->useProgram(); }
        // Get the programaID of this specific object
        GLuint GetProgramObject() { return shader->getProgramObject(); }
    };
}
