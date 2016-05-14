#include <exception>

#include "interaction.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace RenderingEngine;
using std::terminate;

int main(int argc, char *argv[])
{
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
        terminate();

    // Initialize the needed OpenGL ES characteristics
    if (Init(esContext.width, esContext.height) == GL_FALSE)
        terminate();

    // Set the callback for the draw method
    esRegisterDrawFunc(&esContext, Draw);

    // Set the callback for the keyboard method
    esRegisterKeyFunc(&esContext, Key);

    // Set the callback for the update method
    esRegisterUpdateFunc(&esContext, Update);

    // Enter on the application main loop
    esMainLoop(&esContext);

    ShutDown(&esContext);

    return 0;
}
