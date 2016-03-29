#include <fstream>
#include "shader.h"

namespace RenderingEngine
{
    // Constructor of the shaders that initializes the vertex and fragment shaders
    Shader::Shader(const string &vertexShaderFileName, const string &fragmentShaderFileName, 
                   const vector<unique_ptr<Variable>> &attributesNames, const vector<unique_ptr<Variable>> &uniformsNames)
    {
        Log << Function << endl;

        Log << Debug << "Loading vertex and fragment shaders." << endl;
        // Load the vertex and fragment shaders
        vertexShader = LoadShader(GL_VERTEX_SHADER, vertexShaderFileName);
        // Check if the vertex shader was created correctly
        if (vertexShader == GL_FALSE)
        {
            Log << Error << "There was a problem with the vertex shader creation." << endl;
            exit(1);
        }
        Log << Debug << "Vertex shader loaded correctly." << endl;

        fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentShaderFileName);
        // Check if the fragment shader was created correctly
        if (fragmentShader == GL_FALSE)
        {
            Log << Error << "There was a problem with the fragment shader creation." << endl;
            exit(1);
        }
        Log << Debug << "Fragment shader loaded correctly." << endl;

        // Create the program object
        Log << Debug << "Creating the program object for the shaders." << endl;
        programObject = glCreateProgram();

        // Check if the program object was created correctly
        if (programObject == GL_FALSE)
        {
            Log << Error << "There was a problem at the program object creation." << endl;
            exit(1);
        }

        // Attach the shaders to the program object
        Log << Debug << "Attaching shaders to the program object." << endl;
        glAttachShader(programObject, vertexShader);
        glAttachShader(programObject, fragmentShader);

        // Link the program object
        Log << Debug << "Linking the program object." << endl;
        glLinkProgram(programObject);

        // Check the link status of the program object
        GLint linked;
        glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE)
        {
            // Get information about the error
            GLint infoLen = 0;
            glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1)
            {
                char* infoLog = (char*)malloc(sizeof(char) * infoLen);

                glGetProgramInfoLog(programObject, infoLen, nullptr, infoLog);
                Log << Error << "Could not link the program object:" << endl << infoLog << endl;

                free(infoLog);
            }

            glDeleteProgram(programObject);
            exit(1);
        }

        // Get the location of the attributes and enable the attribute arrays
        if (attributesNames.size() != 0)
        {
            for (auto &attribute : attributesNames)
            {
                attributes.push_back(make_unique<Variable>(attribute->getVariableName(), glGetAttribLocation(programObject, attribute->getVariableName().c_str()),
                                                           attribute->getSize(), attribute->getBufferType()));
                glEnableVertexAttribArray(attributes.back()->getVariableIndex());
            }
        }
        // Get the location of the uniforms
        if (uniformsNames.size() != 0)
        {
            for (auto &uniform : uniformsNames)
                uniforms.push_back(make_unique<Variable>(uniform->getVariableName(), glGetUniformLocation(programObject, uniform->getVariableName().c_str())));
        }
    }

    Shader::~Shader()
    {
        Log << Function << endl;
        // Delete the program object created previously
        glDeleteProgram(programObject);
    }

    // Method to load a shader into the OpenGL ES application
    GLuint Shader::LoadShader(GLenum type, const string &shaderFileName)
    {
        Log << Function << endl;
        string shaderCode, line = "";
        // Read the shader file from disk
        ifstream shaderStream(shaderFileName, ios::in);
        if (shaderStream.is_open())
        {
            // Read all the content of the shader file
            while (getline(shaderStream, line))
                shaderCode += line + "\n";
            // Close the input stream
            shaderStream.close();
        }
        else
        {
            Log << Error << "Unable to read the source code of the shader file: " << shaderFileName << endl;
            return GL_FALSE;
        }

        // Create the shader object of the specified type
        GLuint shader = glCreateShader(type);

        // Check if the creation of the shader was correct
        if (shader == GL_FALSE)
            return GL_FALSE;

        // Load the shader source code
        const char *shaderSrc = shaderCode.c_str();
        glShaderSource(shader, 1, &shaderSrc, nullptr);

        // Compile the shader
        glCompileShader(shader);

        // Check the compilation status of the shader
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE)
        {
            // Get information about the error
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1)
            {
                char* infoLog = (char*)malloc(sizeof(char) * infoLen);

                glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
                Log << Error << "Could not compile shader " << shaderFileName << endl << infoLog << endl;

                free(infoLog);
            }

            glDeleteShader(shader);
            return GL_FALSE;
        }

        // Return the shader number
        return shader;
    }
}
