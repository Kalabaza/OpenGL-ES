#include "mesh.h"

namespace RenderingEngine
{
    Mesh::Mesh(const string& objectFileName, unique_ptr<Shader> &shader) : shader{ shader }
    {
        Log << Function << endl;

        obj = make_unique<Object>(objectFileName);
        
        Log << Debug << "Object file \"" << objectFileName << "\" loaded to memory." << endl;

        // Load the obtained information into a VBO (Vertex Buffer Object)
        Log << Debug << "Loading Vertex, UV and Normals information into a VBO." << endl;

        // Vertex information
        glGenBuffers(1, &vertexBuffer);
        // Bind the buffer with the array buffer binding point
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, obj->getSize() * sizeof(Vertex), obj->verticesData().data(), GL_STATIC_DRAW);

        // Indexes information
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->getSize() * sizeof(GLushort), obj->indexData().data(), GL_STATIC_DRAW);

        Log << Info << "Object " << objectFileName << " loaded correctly." << endl;
    }

    Mesh::~Mesh()
    {
        Log << Function << endl;
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &indexBuffer);
    }

    void Mesh::draw()
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

        for (auto &attribute : shader->getAttributes())
        {
            glEnableVertexAttribArray(attribute->getVariableIndex());
            glVertexAttribPointer(
                attribute->getVariableIndex(),                  // attribute
                attribute->getSize(),                           // size
                GL_FLOAT,                                       // type
                GL_FALSE,                                       // normalized?
                sizeof(Vertex),                                 // stride
                (attribute->getBufferType() == Vertices) 
                ? (GLvoid*)0 : (GLvoid*)(sizeof(float) * 3)     // array buffer offset
            );
        }

        glDrawElements(GL_TRIANGLES, obj->getSize(), GL_UNSIGNED_SHORT, 0);
    }
}
