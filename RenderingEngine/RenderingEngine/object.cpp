#include "object.h"

namespace RenderingEngine
{
    // Constructor to make a skybox object
    Object::Object()
    {
        GLfloat skyboxVertices[] = {
            //  x,     y,     z,     x,     y,     z,     x,     y,     z,     x,     y,     z,     x,     y,     z,     x,     y,     z
            -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  // +X
            -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  // -X
             1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  // +Y
            -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  // -Y
            -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,  // +Z
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f   // -Z
        };
        for (int index = 0; index < sizeof(skyboxVertices) / sizeof(float); index += 3)
        {
            indexes.push_back(index);
            vertices.push_back(Vertex(vec3(skyboxVertices[index], skyboxVertices[index + 1], skyboxVertices[index + 2]),
                                      vec2(0.0f, 0.0f),             // Texture coordinates are not needed
                                      vec3(0.0f, 0.0f, 0.0f)));     // Normals just ignored for now
        }
    }

    // Constructor for a normal object (read from disk)
    Object::Object(const string &objectFileName)
    {
        Log << Function << endl;
        // String to hold a line read from disk
        string line;
        // Open the input file
        ifstream objFile(objectFileName, ios::in);
        // Check if the file was opened correctly
        if (!objFile.is_open())
        {
            Log << Error << "Unable to read the object file: " << objectFileName << endl;
            exit(1);
        }

        // Temporary location for the information of the file
        vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

        // Temporary vectors to hold the data read from disk
        vector<vec3> tmpVertices, tmpNormals;
        vector<vec2> tmpUvs;

        while (getline(objFile, line))
        {
            // If there's a diagonal in the text, change that for spaces to avoid 
            // changing the separator of the stringstreamer
            if (find(line.begin(), line.end(), '/') != line.end())
                replace(line.begin(), line.end(), '/', ' ');

            vec3 vertex, normal;
            vec2 uv;
            // Tokenize the input line
            stringstream ssLine(line);
            string type;
            ssLine >> type;

            // Vertex
            if (type == "v")
            {
                ssLine >> vertex.x >> vertex.y >> vertex.z;
                tmpVertices.push_back(vertex);
            }
            // Vertex Texture
            else if (type == "vt")
            {
                ssLine >> uv.x >> uv.y;
                tmpUvs.push_back(uv);
            }
            // Vertex Normal
            else if (type == "vn")
            {
                ssLine >> normal.x >> normal.y >> normal.z;
                tmpNormals.push_back(normal);
            }
            // Faces
            else if (type == "f")
            {
                // The faces come in the following format
                // vertex/texture/normal vertex/texture/normal vertex/texture/normal
                // Each line represents a hole triangle
                ssLine >> vertexIndex[0] >> uvIndex[0] >> normalIndex[0]
                       >> vertexIndex[1] >> uvIndex[1] >> normalIndex[1]
                       >> vertexIndex[2] >> uvIndex[2] >> normalIndex[2];
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
            // Probably just a comment or empty line, just ignore it and get the next line
            else
                continue;
        }
        // Close the input file
        objFile.close();

        // Insert each vertex of the triangles
        for (size_t index = 0; index < vertexIndices.size(); ++index)
        {
            indexes.push_back((GLushort)index);
            vertices.push_back(Vertex(tmpVertices[vertexIndices[index] - 1],
                                     // Check if the texture coordinates were provided in the object file
                                     (tmpUvs.size() != 0) ? tmpUvs[uvIndices[index] - 1] : vec2(0.0f, 0.0f),
                                     // Check if the normal vectors were provided in the object file
                                     (tmpNormals.size() != 0) ? tmpNormals[normalIndices[index] - 1] : vec3(0.0f, 0.0f, 0.0f)));
        }

        // Clear the temporary vectors
        vertexIndices.clear();
        uvIndices.clear();
        normalIndices.clear();
        tmpVertices.clear();
        tmpUvs.clear();
        tmpNormals.clear();
    }
}
