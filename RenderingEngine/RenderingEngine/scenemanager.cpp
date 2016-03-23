#include "scenemanager.h"

namespace RenderingEngine
{
    SceneManager::SceneManager(ESContext *esContext)
    {
        Log << Function << endl;

        // Read the resources.txt file to obtain the valid configuration for the engine
        string resourcesFileName = "resources.txt";
        ifstream resourcesFile(resourcesFileName, ios::in);
        if (!resourcesFile.is_open())
        {
            Log << Error << "Unable to read the resources file: " << resourcesFileName << endl;
            exit(1);
        }

        Log << Debug << "Parsing the resources.txt file." << endl;
        string line;
        string name, vertexShader, fragmentShader;
        unsigned int index;
        GLuint size, bufferType;
        float posX, posY, posZ, sclX, sclY, sclZ, angle, rotX, rotY, rotZ;
        char token;
        while (getline(resourcesFile, line))
        {
            stringstream ssLine(line);
            // Ignore empty lines on the configuration file
            if(line.size() == 0)
                continue;
            ssLine >> token;
            switch (token)
            {
            // Attributes used on the shaders
            case 'A':
                ssLine >> name >> size >> bufferType;
                attributes.push_back(make_unique<Variable>(name, size, (BufferType)bufferType));
                break;
            // Uniforms used on the shaders
            case 'U':
                ssLine >> name;
                uniforms.push_back(make_unique<Variable>(name));
                break;
            // Shaders
            case 'S':
                ssLine >> vertexShader >> fragmentShader;
                shader.push_back(make_tuple(vertexShader, fragmentShader));
                break;
            // Object definitions
            case 'O':
                ssLine >> name >> index;
                mesh.push_back(make_tuple(name, index));
                break;
            // Textures
            case 'T':
                ssLine >> name >> index;
                texture.push_back(make_tuple(name, index));
                break;
            // Initial coordinates
            case 'C':
                ssLine >> posX >> posY >> posZ >> sclX >> sclY >> sclZ >> angle >> rotX >> rotY >> rotZ;
                position.push_back(vec3(posX, posY, posZ));
                scale.push_back(vec3(sclX, sclY, sclZ));
                rotation.push_back(vec3(rotX, rotY, rotZ));
                break;
            // Projection matrix
            case 'P':
                ssLine >> projectionName;
                break;
            // Modelview matrix
            case 'M':
                ssLine >> modelviewName;
                break;
            // If the line is a comment of something else get the next token
            case '#':
            default:
                continue;
            }
        }
        resourcesFile.close();

        // First add the mesh, shaders and texture of each object
        Log << Debug << "Loading shaders, objects and textures." << endl;

        for (size_t index = 0; index < shader.size(); ++index)
        {
            shaders.push_back(make_unique<Shader>(get<0>(shader[index]), get<1>(shader[index]), attributes, uniforms));
        }

        for (size_t index = 0; index < mesh.size(); ++index)
        {
            
            meshes.push_back(make_unique<Mesh>(get<0>(mesh[index]), shaders[get<1>(mesh[index])]));
            sceneobjects.push_back(make_unique<SceneObject>(position[index], scale[index], angle, rotation[index]));
        }

        for (size_t index = 0; index < texture.size(); ++index)
        {
            textures.push_back(make_unique<Texture>(get<0>(texture[index]), shaders[get<1>(texture[index])]));
        }

        // The projection matrix is represented by the perspective matrix given by glm, assign it to each one of the objects
        GLfloat aspect = static_cast<GLfloat>(esContext->width) / static_cast<GLfloat>(esContext->height);
        projection = glm::perspective(
            45.0f,      // Field of view, is the amount of zoom. A wide angle is 90 and a narrow angle is 30
            aspect,     // Depends on the size of the window
            0.1f,       // Near clipping plane
            100.0f      // Far clipping plane
            );

        camera = vec3(0.0f, 0.0f, -5.0f);

        projectionUniform = glGetUniformLocation(shaders[0]->getProgramObject(), "Projection");
        modelviewUniform = glGetUniformLocation(shaders[index]->getProgramObject(), "Modelview");

        shader.clear();
        mesh.clear();
        position.clear();
        scale.clear();
        rotation.clear();
        texture.clear();
    }

    SceneManager::~SceneManager()
    {
        Log << Function << endl;
        shaders.clear();
        meshes.clear();
        sceneobjects.clear();
        textures.clear();
        //mvpIndexes.clear();
    }

    void SceneManager::draw()
    {
        // Update the projection matrix
        glUniformMatrix4fv(projectionUniform, 1, 0, &projection[0][0]);
        for (size_t index = 0; index < meshes.size(); ++index)
        {
            // Update the Modelview matrix
            glUniformMatrix4fv(modelviewUniform, 1, 0, &sceneobjects[index]->getModelTransform()[0][0]);
            
            meshes[index]->draw();
            textures[index]->draw();
        }
    }

    void SceneManager::update(ESContext *esContext)
    {
        for (size_t index = 0; index < sceneobjects.size(); ++index)
        {
            sceneobjects[index]->updateModelTransform(camera, angle);
        }
    }
}
