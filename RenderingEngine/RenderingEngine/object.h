#pragma once

// STD
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <exception>

// GLM
#include <glm/glm.hpp>

// OpenGL
#include <GLES2/gl2.h>

#if defined(__ANDROID__)
#include <android/asset_manager.h>
#endif

// Engine
#include "logger.h"

namespace RenderingEngine
{
    using std::vector;
    using std::string;
    using std::ifstream;
    using std::ios;
    using std::stringstream;
    using std::terminate;
    using glm::vec3;
    using glm::vec2;

    // 
    typedef struct _Vertex_ {
        vec3 Position;  // Three coordinates for position X,Y,Z
        vec2 TexCoord;  // Texture has two coordinates U,V
        vec3 Normals;   // Normals have three coordinates X,Y,Z
        _Vertex_(const vec3 &Pos, const vec2 &Tex, vec3 Nor) : Position{ Pos }, TexCoord{ Tex }, Normals{ Nor } {}
    } Vertex;

    class Object
    {
    private:
        vector<Vertex> vertices;
        vector<GLushort> indexes;

    public:
        // Constructor for a skymap
        Object();
        // Constructor for a normal object
#if defined(__ANDROID__)
        Object(AAssetManager**, const string&);
#else
        Object(const string&);
#endif

        // Get the number of vertices on the object
        int getSize() { return vertices.size(); }

        // Get access to the vertices and indexes vectors holding the data of the object file
        Vertex *verticesData() { return vertices.data(); }
        GLushort *indexData()  { return indexes.data(); }
    };
}
