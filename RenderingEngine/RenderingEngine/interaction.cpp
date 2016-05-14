#include "interaction.h"

namespace RenderingEngine
{
    // Method to initialize the OpenGL related information
#ifdef __ANDROID__
    void Java_mx_iteso_msc_rodriguez_roberto_renderingengine_CustomGLRenderer_Init(JNIEnv *env, jobject object, jobject assetManager, jint width, jint height)
#else
    GLuint Init(GLint width, GLint height)
#endif
    {
#if defined(__ANDROID__)
        Log << All << Function << endl;
#else
        Log << Function << endl;
#endif

        // Dark blue background
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // Enable the depth test
        glEnable(GL_DEPTH_TEST);

        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);

        // Load the objects using the scene manager
#if defined (__ANDROID__)
        sceneManager = make_unique<SceneManager>(&env, assetManager, width, height);
#else
        sceneManager = make_unique<SceneManager>(width, height);
#endif

        Log << Info << "Finished initialization of OpenGL elements." << endl;
#ifndef __ANDROID__
        return GL_TRUE;
#endif
    }

    // Method used to draw on screen
#ifdef __ANDROID__
    void Java_mx_iteso_msc_rodriguez_roberto_renderingengine_CustomGLRenderer_Draw(JNIEnv *env, jobject object, jint width, jint height)
#else
    void Draw(ESContext *esContext)
#endif
    {
        // Set the viewport to the size of the window
#ifdef __ANDROID__
        glViewport(0, 0, width, height);
#else
        glViewport(0, 0, esContext->width, esContext->height);
#endif

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the objects and textures on screen
        sceneManager->Draw();

        // Swap display buffers
#ifndef __ANDROID__
        eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
#endif
    }

#ifndef __ANDROID__
    // Method handle keyboard pressed keys
    void Key(ESContext *esContext, unsigned char key, int x, int y)
    {
        // Current implementation of the camera, needs to be improved
        vec3 camera = sceneManager->getCamera();
        switch (key)
        {
            // Scape key pressed, exit the application
        case 27:
            Log << Debug << "Escape key pressed, sending quit message." << endl;
#if defined(_WIN32)
            PostQuitMessage(0);
#elif defined(__linux__)
            exit(0);
#endif
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
#endif

#ifdef __ANDROID__
    void Java_mx_iteso_msc_rodriguez_roberto_renderingengine_CustomGLSurfaceView_Update(JNIEnv *env, jobject object)
#else
    void Update()
#endif
    {
        // Update the objects on screen based on changes on the rendering area or any user interaction
        sceneManager->Update();
    }

#ifdef __ANDROID__
    void Java_mx_iteso_msc_rodriguez_roberto_renderingengine_MainFragment_Shutdown(JNIEnv *env, jobject object)
#else
    void ShutDown(ESContext *esContext)
#endif
    {
        Log << Function << endl;
        sceneManager->Clean();
#ifndef __ANDROID__
        // Delete the program created on OpenGL
        glDeleteProgram(esContext->programID);
        // Destroy the window
        esDestroyWindow(esContext);
#endif
    }
}