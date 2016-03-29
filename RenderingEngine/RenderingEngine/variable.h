#pragma once

// STD
#include <string>

// OpenGL
#include <GLES2/gl2.h>

namespace RenderingEngine
{
    using std::string;

    enum BufferType
    {
        Vertices = 0,
        Textures,
        Normals,
        NotSet
    };


    // Class used to send the attributes and uniforms between the objects
    class Variable
    {
    private:
        string variableName;
        GLint variableIndex;
        GLuint size;
        BufferType bufferType;
    public:
        Variable(const string &variableName) : variableName{ variableName }, size{ 0 }, bufferType{ NotSet } {}
        Variable(const string &variableName, const GLint &variableIndex) : variableName{ variableName }, variableIndex{ variableIndex }, size{ 0 }, bufferType{ NotSet } {}
        Variable(const string &variableName, GLuint size, BufferType bufferType) : variableName{ variableName }, size{ size }, bufferType{ bufferType } {}
        Variable(const string &variableName, const GLint variableIndex, GLuint size, BufferType bufferType) : variableName{ variableName }, variableIndex { variableIndex }, size{ size }, bufferType{ bufferType } {}

        string getVariableName() { return variableName; }
        GLint getVariableIndex() { return variableIndex; }
        GLuint getSize() { return size; }
        BufferType getBufferType() { return bufferType; }
    };
}