#pragma once

#include <vector>
#include <memory>

// OpenGL
#include <GLES2/gl2.h>

// Engine
#include "logger.h"
#include "variable.h"

namespace RenderingEngine
{
    using std::vector;
    using std::unique_ptr;
    using std::make_unique;
    using std::string;
    using std::ifstream;
    using std::ios;

    // Class to hold a vertex and fragment shader
    class Shader
    {
    private:
        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint programObject;
        vector<unique_ptr<Variable>> attributes;
        vector<unique_ptr<Variable>> uniforms;

        GLuint LoadShader(GLenum, const string &);
    public:
        Shader() = delete;
        Shader(const string &, const string&, const vector<unique_ptr<Variable>>&, const vector<unique_ptr<Variable>>&);
        ~Shader();

        GLuint getProgramObject() { return programObject; }
        vector<unique_ptr<Variable>> &getAttributes() { return attributes; }
        vector<unique_ptr<Variable>> &getUniforms() { return uniforms; }
    };
}
