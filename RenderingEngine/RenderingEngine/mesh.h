#pragma once

// STD
#include <vector>
#include <memory>

// GLM
#include <glm/glm.hpp>

// OpenGL
#include <GLES2/gl2.h>

// Engine
#include "logger.h"
#include "object.h"
#include "shader.h"

namespace RenderingEngine
{
    using std::vector;
    using std::unique_ptr;
    using std::make_unique;
    using std::string;
    using glm::vec3;
    using glm::vec2;

    class Mesh
    {
    private:
        unique_ptr<Object> obj;

        // Buffer objects handle
        GLuint vertexBuffer;
        GLuint indexBuffer;

        unique_ptr<Shader> &shader;

    public:
        Mesh() = delete;
        Mesh(const string&, unique_ptr<Shader>&);
        ~Mesh();
        void draw();
    };
}
