#include "mesh.h"

namespace RenderingEngine
{
    Mesh::Mesh(const string &objectFileName, shared_ptr<Shader> &shader, unique_ptr<Texture> *texture /*= nullptr*/) : shader{ shader }
    {
        Log << Function << endl;

        if (objectFileName.empty())
            // Create an skybox object, this is an special kind of object
            obj = make_unique<Object>();
        else
            // Create the object and try to load the file
            obj = make_unique<Object>(objectFileName);
        
        Log << Debug << "Object file \"" << objectFileName << "\" loaded to memory." << endl;

        // If the texture was received, then the hightmap will be calculated using the image values
        if (texture != nullptr)
        {
            // Pointer to the first vertex
            Vertex *vertex = obj->verticesData();
            unsigned char *pixels = (*texture)->TextureData();
            GLint width = (*texture)->GetWidth();
            GLint height = (*texture)->GetHeight();
            // Change the Y value of all the vertices
            for (int index = 0; index < obj->getSize(); ++index)
            {
                // Get the value of the X and Y that will be used inside of the texture
                int x = static_cast<int>(vertex[index].TexCoord.x * (width - 1));
                int y = static_cast<int>(vertex[index].TexCoord.y * (height - 1));
                // Calculate the offset to use in the texture to get the right RGB value (3 bytes of data for pixel)
                int nPos = (x + y * width) * 3;
                // The Y value will be calculated with the relative luminance algorithm and will use just the 5% of the result value
                vertex[index].Position.y = 0.05f * (0.2126f * static_cast<float>(pixels[nPos]) +
                                                    0.7152f * static_cast<float>(pixels[nPos + 1]) +
                                                    0.0722f * static_cast<float>(pixels[nPos + 2]));
            }
        }

        // Load the vertices and indices into a VBO (Vertex Buffer Object)
        // Vertices information
        glGenBuffers(1, &vertexBuffer);
        // Bind the vertices buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        // Send the vertex data to OpenGL
        glBufferData(GL_ARRAY_BUFFER, obj->getSize() * sizeof(Vertex), obj->verticesData(), GL_STATIC_DRAW);

        // Indexes information
        glGenBuffers(1, &indexBuffer);
        // Bind the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        // Send the index data to OpenGL
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->getSize() * sizeof(GLushort), obj->indexData(), GL_STATIC_DRAW);

        Log << Info << "Object " << objectFileName << " loaded correctly." << endl;
    }

    Mesh::~Mesh()
    {
        Log << Function << endl;
        // Delete the buffers at application end
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &indexBuffer);
    }

    void Mesh::Draw()
    {
        // Bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        // Check if there's any attributes needing data
        for (auto &attribute : shader->getAttributes())
        {
            // Only enable valid attributes on a specific shader
            if (attribute->getVariableIndex() >= 0)
            {
                glVertexAttribPointer(
                    attribute->getVariableIndex(),                  // attribute
                    attribute->getSize(),                           // size
                    GL_FLOAT,                                       // type
                    GL_FALSE,                                       // normalized?
                    sizeof(Vertex),                                 // stride
                    (attribute->getBufferType() == Vertices) ? (GLvoid*)0 :                         // Use the Vertices data
                   ((attribute->getBufferType() == Textures) ? (GLvoid*)(sizeof(float) * 3) :       // Use the Textures data
                                                               (GLvoid*)(sizeof(float) * 5)));      // Use the Normals data
            }
        }
        // Bind the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

        // Draw the object on screen
        glDrawElements(GL_TRIANGLES, obj->getSize(), GL_UNSIGNED_SHORT, 0);
    }
}
