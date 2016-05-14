#pragma once

// STD
#include <memory>

// Engine
#include "logger.h"
#if defined (__ANDROID__)
#include <jni.h>
#else
#include "openGLES.h"
#endif
#include "scenemanager.h"

namespace RenderingEngine
{
    using std::unique_ptr;
    using std::make_unique;

    // Size of the rendering window
    const int Width = 1024;
    const int Height = 768;

    static unique_ptr<SceneManager> sceneManager;

    static GLuint tertureBuffer;

    // Method to initialize the OpenGL ES related information
#ifdef __ANDROID__
extern "C"{
    JNIEXPORT void JNICALL Java_mx_iteso_msc_rodriguez_roberto_renderingengine_CustomGLRenderer_Init(JNIEnv*, jobject, jobject, jint, jint);
};
#else
    GLuint Init(GLint, GLint);
#endif

    // Method used to draw on screen
#ifdef __ANDROID__
extern "C" {
    void Java_mx_iteso_msc_rodriguez_roberto_renderingengine_CustomGLRenderer_Draw(JNIEnv*, jobject, jint, jint);
};
#else
    void Draw(ESContext*);
#endif

#ifndef __ANDROID__
    // Method handle keyboard pressed keys
    void Key(ESContext*, unsigned char, int, int);
#endif

#ifdef __ANDROID__
extern "C" {
    void Java_mx_iteso_msc_rodriguez_roberto_renderingengine_CustomGLSurfaceView_Update(JNIEnv*, jobject);
};
#else
    void Update();
#endif

    // Cleanup method
#ifdef __ANDROID__
extern "C" {
    void Java_mx_iteso_msc_rodriguez_roberto_renderingengine_MainFragment_Shutdown(JNIEnv*, jobject);
};
#else
    void ShutDown(ESContext*);
#endif
}
