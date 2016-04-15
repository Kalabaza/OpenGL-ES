#include "scenemanager.h"

namespace RenderingEngine
{
    SceneManager::SceneManager(ESContext *esContext)
    {
        Log << Function << endl;

        // The projection matrix is represented by the perspective matrix given by glm, assign it to each one of the objects
        GLfloat aspect = static_cast<GLfloat>(esContext->width) / static_cast<GLfloat>(esContext->height);
        projectionMatrix = glm::perspective(
            45.0f,      // Field of view, is the amount of zoom. A wide angle is 90 and a narrow angle is 30
            aspect,     // Depends on the size of the window
            0.1f,       // Near clipping plane
            500.0f      // Far clipping plane
        );

        // Read the resources.txt file to obtain the valid configuration for the engine
        string resourcesFileName = "resources.txt";
        ifstream resourcesFile(resourcesFileName, ios::in);
        if (!resourcesFile.is_open())
        {
            Log << Error << "Unable to read the resources file: " << resourcesFileName << endl;
            exit(1);
        }

        Log << Debug << "Parsing the resources.txt file." << endl;
        string line, name, vertex, fragment, object, texture, projection, modelview;
        vector<string> cubeTextures{ NumCubeFaces };
        GLuint size, bufferType;
        vec3 pos, scl, rot;
        char token;
        bool finished = true;

        while (getline(resourcesFile, line))
        {
            stringstream ssLine(line);
            // Ignore empty lines on the configuration file
            if(line.size() == 0)
                continue;

            ssLine >> token;
            switch (token)
            {
            // If the line is a comment get the next token
            case '#':
                continue;
            // Start of object definition
            case '.':
                // Create a new scene object
                Log << Debug << "Starting an object definition." << endl;
                sceneobjects.push_back(make_unique<SceneObject>());
                finished = false;
                break;
            // End of an object definition
            case '-':
                Log << Debug << "End an object definition." << endl;
                finished = true;
                break;
            // Attributes used on the shaders
            case 'A':
                ssLine >> name >> size >> bufferType;
                Log << Debug << "Adding the attribute: " << name << endl;
                attributes.push_back(make_unique<Variable>(name, size, (BufferType)bufferType));
                break;
            // Uniforms used on the shaders
            case 'U':
                ssLine >> name;
                Log << Debug << "Adding the uniform: " << name << endl;
                uniforms.push_back(make_unique<Variable>(name));
                break;
            // Shaders creation
            case 'S':
                ssLine >> vertex >> fragment;
                Log << Debug << "Creating the shaders." << endl;
                sceneobjects.back()->SetShader(make_shared<Shader>(vertex, fragment, attributes, uniforms));
                break;
            // Object definitions
            case 'O':
                ssLine >> object;
                Log << Debug << "Loading a model." << endl;
                sceneobjects.back()->SetMesh(make_unique<Mesh>(object, sceneobjects.back()->GetShader()));
                break;
            // Textures
            case 'T':
                ssLine >> texture;
                Log << Debug << "Loading a texture." << endl;
                sceneobjects.back()->SetTexture(make_unique<Texture>(texture, sceneobjects.back()->GetShader()));
                break;
            // Initial coordinates
            case 'C':
                Log << Debug << "Adding coordinates to the object." << endl;
                ssLine >> pos.x >> pos.y >> pos.z >> scl.x >> scl.y >> scl.z >> rot.x >> rot.y >> rot.z >> angle;
                sceneobjects.back()->SetCoordinates(pos, scl, rot, angle);
                break;
            // Projection matrix
            case 'P':
                ssLine >> projection;
                break;
            // Modelview matrix
            case 'M':
                ssLine >> modelview;
                break;
            // Skybox
            case 'B':
                Log << Debug << "Adding a skybox." << endl;
                ssLine >> cubeTextures[0] >> cubeTextures[1] >> cubeTextures[2] >> cubeTextures[3] >> cubeTextures[4] >> cubeTextures[5];
                sceneobjects.back()->SetSkymap();
                sceneobjects.back()->SetMesh(make_unique<Mesh>(string(""), sceneobjects.back()->GetShader()));
                sceneobjects.back()->SetTexture(make_unique<Texture>(cubeTextures, sceneobjects.back()->GetShader()));
                break;
            // Terrain Heightmap
            case 'H':
                Log << Debug << "Loading the terrain." << endl;
                ssLine >> texture >> object;
                sceneobjects.back()->SetTexture(make_unique<Texture>(texture, sceneobjects.back()->GetShader()));
                sceneobjects.back()->SetMesh(make_unique<Mesh>(object, sceneobjects.back()->GetShader(), &sceneobjects.back()->GetTexture()));
                break;
            default:
                continue;
            }

            // Check if the definition of an object is complete or if more lines are needed
            if (finished)
            {
                // Get the projection and modelview uniforms
                if (!projection.empty())
                {
                    sceneobjects.back()->SetProjectionUni(glGetUniformLocation(sceneobjects.back()->GetShader()->getProgramObject(), "Projection"));
                    projection.clear();
                }
                if (!modelview.empty())
                {
                    sceneobjects.back()->SetModelviewUni(glGetUniformLocation(sceneobjects.back()->GetShader()->getProgramObject(), "Modelview"));
                    modelview.clear();
                }

                // Clear the attributes and uniforms in order to load the next object
                attributes.clear();
                uniforms.clear();
            }
        }
        // Close de the resources file
        resourcesFile.close();

        // Set the initial position of the camera
        camera = vec3(2.5f, -1.0f, -5.0f);

        // Initial value of the rotation angle
        angle = 0.0f;
    }

    void SceneManager::Clean()
    {
        Log << Function << endl;
        // Clean all the screen objects, all the destructors will be called
        sceneobjects.clear();
    }

    void SceneManager::Draw()
    {
        for (size_t index = 0; index < sceneobjects.size(); ++index)
        {
            // Enable the program before starting sending information to OpenGL
            sceneobjects[index]->GetShader()->useProgram();
            // Update the projection matrix
            GLint projectionUniform = sceneobjects[index]->GetProjectionUni();
            if (projectionUniform != -1)
            {
                glUniformMatrix4fv(projectionUniform, 1, 0, value_ptr(projectionMatrix));
            }
            // Update the Modelview matrix
            GLint modelviewUniform = sceneobjects[index]->GetModelviewUni();
            if (modelviewUniform != -1)
            {
                if (sceneobjects[index]->IsSkymap())
                {
                    mat4 proj = glm::lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
                    mat4 view = glm::mat4(glm::mat3(proj));
                    glUniformMatrix4fv(modelviewUniform, 1, 0, value_ptr(view));
                }
                else
                    glUniformMatrix4fv(modelviewUniform, 1, 0, value_ptr(sceneobjects[index]->GetModelviewMatrix()));
            }
            // Activate the texture
            sceneobjects[index]->GetTexture()->Draw((sceneobjects[index]->IsSkymap()) ? CubeMap : SingleTexture);
            // Draw the triangles of an object
            sceneobjects[index]->GetMesh()->Draw();
        }
    }

    void SceneManager::Update(ESContext *esContext)
    {
        // There was a change on state of the objects, update the modelview matrix
        for (size_t index = 0; index < sceneobjects.size(); ++index)
        {
            sceneobjects[index]->updateModelview(camera, angle);
        }
    }
}
