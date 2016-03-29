#include "main.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace RenderingEngine
{
    // Method to initialize the OpenGL related information
    GLuint Init(ESContext *esContext)
    {
        Log << Function << endl;

        // Dark blue background
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // Enable the depth test
        glEnable(GL_DEPTH_TEST);

        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);

        // Load the objects using the scene manager
        sceneManager = make_unique<SceneManager>(esContext);

        Log << Info << "Finished initialization of OpenGL elements." << endl;
        return GL_TRUE;
    }

    // Method used to draw on screen
    void Draw(ESContext *esContext)
    {
        // Set the viewport to the size of the window
        glViewport(0, 0, esContext->width, esContext->height);

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the objects and textures on screen
        sceneManager->Draw();
        
        // Swap display buffers
        eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
    }

    // Method handle keyboard pressed keys
    void Key(ESContext *esContext, unsigned char key, int x, int y)
    {
        // Current implementation of the camera, needs to be improved
        vec3 camera = sceneManager->getCamera();
        switch (key)
        {
            // Scape key pressed, exit the application
        case 27:
            Log << Debug << "Escape key pressed, finishing application." << endl;
            glDeleteProgram(esContext->programID);
            exit(0);
            break;
        case 'a':
            camera.x -= 0.5;
            break;
        case 's':
            camera.x += 0.5;
            break;
        case 'w':
            camera.y += 0.5;
            break;
        case 'z':
            camera.y -= 0.5;
            break;
        case 'e':
            camera.z += 0.5;
            break;
        case 'd':
            camera.z -= 0.5;
            break;
        case 'r':
            {
                float angle = sceneManager->getAngle();
                angle += 0.5f;
                if (angle >= 360.0f)
                    angle -= 360.0f;
                sceneManager->setAngle(angle);
                return;
            }
        case 'f':
            {
                float angle = sceneManager->getAngle();
                angle -= 0.5f;
                if (angle <= 0.0f)
                    angle += 360.0f;
                sceneManager->setAngle(angle);
                return;
            }
        default:
            break;
        }
        sceneManager->setCamera(camera);
    }

    void Update(ESContext *esContext, float deltaTime)
    {
        // Update the objects on screen based on changes on the rendering area or any user interaction
        sceneManager->Update(esContext);
    }
}

int main(int argc, char *argv[])
{
    using namespace RenderingEngine;

    // Change the logging level to show all the messages
    Log << All << Function << endl;

    // OpenGL ES context data
    ESContext esContext;

    // Initialize the OpenGL ES context to zeros
    Log << Debug << "Initializing the OpenGL ES context." << endl;
    esInitContext(&esContext);

    // Create a render surface with the specified characteristics
    Log << Debug << "Creating a window for rendering purposes." << endl;
    // Allocate RGB color buffer and depth buffer
    if (esCreateWindow(&esContext, "Rendering Engine", Width, Height, ES_WINDOW_RGB | ES_WINDOW_DEPTH) == GL_FALSE)
        exit(1);

    // Initialize the needed OpenGL ES characteristics
    if (Init(&esContext) == GL_FALSE)
        exit(1);

    // Set the callback for the draw method
    esRegisterDrawFunc(&esContext, Draw);

    // Set the callback for the keyboard method
    esRegisterKeyFunc(&esContext, Key);

    // Set the callback for the update method
    esRegisterUpdateFunc(&esContext, Update);

    // Enter on the application main loop
    esMainLoop(&esContext);

    return 0;
}
