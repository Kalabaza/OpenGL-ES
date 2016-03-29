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
        GLint programObject;
        vector<unique_ptr<Variable>> attributes;
        vector<unique_ptr<Variable>> uniforms;

        GLuint LoadShader(GLenum, const string &);
    public:
        // Disable the default constructor
        Shader() = delete;
        Shader(const string &, const string&, const vector<unique_ptr<Variable>>&, const vector<unique_ptr<Variable>>&);
        ~Shader();
        // Deep copy constructor
        Shader(const Shader &original)
        {
            vertexShader = original.vertexShader;
            fragmentShader = original.fragmentShader;
            programObject = original.programObject;
            for (auto &att : original.attributes)
                attributes.push_back(make_unique<Variable>(*att));

            for (auto &uni : original.uniforms)
                uniforms.push_back(make_unique<Variable>(*uni));
        }
        // Get the programID overloading the () operator
        operator GLint() { return programObject; }

        // Enable the programID before any draw call
        void useProgram() { glUseProgram(programObject); }
        // Get the programID
        GLint getProgramObject() { return programObject; }

        // Get the attributes and the uniforms
        vector<unique_ptr<Variable>> &getAttributes() { return attributes; }
        vector<unique_ptr<Variable>> &getUniforms() { return uniforms; }
    };
}
