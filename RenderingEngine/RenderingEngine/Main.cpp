#include "Main.h"

// Method to initialize the OpenGL ES related information
GLuint Init(ESContext *esContext)
{
    // Vertex shader
    GLbyte vShaderStr[] =
        "attribute vec4 vPosition;    \n"
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = vPosition;  \n"
        "}                            \n";

    // Fragment shader
    GLbyte fShaderStr[] =
        "precision mediump float;\n"\
        "void main()                                  \n"
        "{                                            \n"
        "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
        "}                                            \n";

    // Load the vertex and fragment shaders
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, (char*)vShaderStr);
    GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, (char*)fShaderStr);

    // Check if the shaders were created correctly
    if (vertexShader == GL_FALSE || fragmentShader == GL_FALSE)
    {
        std::cerr << "There was a problem at the shaders creation." << std::endl;
        return GL_FALSE;
    }

    // Create the program object
    GLuint programObject = glCreateProgram();

    // Check if the program object was created correctly
    if (programObject == GL_FALSE)
    {
        std::cerr << "There was a problem at the program creation." << std::endl;
        return GL_FALSE;
    }

    // Attach the shaders to the program object
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    // Bind vPosition to attribute 0
    glBindAttribLocation(programObject, 0, "vPosition");

    // Link the program object
    glLinkProgram(programObject);

    GLint linked;

    // Check the link status of the program object
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        // Get information about the error
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);

            glGetProgramInfoLog(programObject, infoLen, nullptr, infoLog);
            std::cerr << "Error linking program: " << infoLog << std::endl;

            free(infoLog);
        }

        glDeleteProgram(programObject);
        return GL_FALSE;
    }

    // Store the program object in the user data of the context
    static_cast<UserData*>(esContext->userData)->programObject = programObject;

    // Set the clear color as black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Load the model of the terrain plan.obj

    return GL_TRUE;
}

// Method used to draw on screen
void Draw(ESContext *esContext)
{
    // Coordinates for the sample triangle
    GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f,
                           -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f };

    // Set the viewport to the size of the window
    glViewport(0, 0, esContext->width, esContext->height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object from the userData in the context
    glUseProgram(static_cast<UserData*>(esContext->userData)->programObject);

    // Load the vertex data into OpenGL ES
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    // Draw the primitives from the array data
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap display buffers
    eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

// Method handle keyboard pressed keys
void Key(ESContext *esContext, unsigned char key, int x, int y)
{
    switch (key)
    {
    // Scape key pressed, exit the application
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    // OpenGL ES context data
    ESContext esContext;
    // User data
    UserData  userData;

    // Initialize the OpenGL ES context to zeros
    esInitContext(&esContext);

    // Assign the local userData to the context
    esContext.userData = &userData;

    // Create a render surface with the specified characteristics
    if (esCreateWindow(&esContext, "Rendering Engine", Width, Height, ES_WINDOW_RGB) == GL_FALSE)
        exit(1);

    // Initialize the needed OpenGL ES characteristics
    if (Init(&esContext) == GL_FALSE)
        exit(1);

    // Set the callback for the draw method
    esRegisterDrawFunc(&esContext, Draw);

    // Set the callback for the keyboard method
    esRegisterKeyFunc(&esContext, Key);

    // Enter on the application main loop
    esMainLoop(&esContext);
}
