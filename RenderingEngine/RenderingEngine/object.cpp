#include <algorithm>

#include "logger.h"

#include "object.h"

namespace RenderingEngine
{
    Object::Object(const string &objectFileName)
    {
        Log << Function << endl;
        string line;
        ifstream objFile(objectFileName, ios::in);
        if (!objFile.is_open())
        {
            Log << Error << "Unable to read the object file: " << objectFileName << endl;
            exit(1);
        }

        // Temporary location for the information of the file
        vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        vector<vec3> tmpVertices;
        vector<vec2> tmpUvs;
        vector<vec3> tmpNormals;

        while (getline(objFile, line))
        {
            // If there's a diagonal in the text, change that for spaces
            if (find(line.begin(), line.end(), '/') != line.end())
                replace(line.begin(), line.end(), '/', ' ');

            stringstream ssLine(line);
            string type;
            ssLine >> type;
            // Vertex
            if (type == "v")
            {
                vec3 vertex;
                ssLine >> vertex.x >> vertex.y >> vertex.z;
                tmpVertices.push_back(vertex);
            }
            // Vertex Texture
            else if (type == "vt")
            {
                vec2 uv;
                ssLine >> uv.x >> uv.y;
                tmpUvs.push_back(uv);
            }
            // Vertex Normal
            else if (type == "vn")
            {
                vec3 normal;
                ssLine >> normal.x >> normal.y >> normal.z;
                tmpNormals.push_back(normal);
            }
            // Faces
            else if (type == "f")
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
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
            // Probably just a comment or empty line, just ignore it
            else
                continue;
        }
        objFile.close();

        // For each vertex of each triangle
        for (size_t index = 0; index < vertexIndices.size(); ++index)
        {
            indexes.push_back((GLushort)index);
            // For now, normals are not used
            vertices.push_back(Vertex(tmpVertices[vertexIndices[index] - 1], tmpUvs[uvIndices[index] - 1], 
                                     (tmpNormals.size() != 0) ? tmpNormals[normalIndices[index] - 1] : vec3(0.0f, 0.0f, 0.0f)));
        }

        vertexIndices.clear();
        uvIndices.clear();
        normalIndices.clear();
        tmpVertices.clear();
        tmpUvs.clear();
        tmpNormals.clear();
    }
}
