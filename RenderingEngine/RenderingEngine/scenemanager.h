#pragma once

// STD
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <tuple>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL
#include <GLES2/gl2.h>

// Engine
#include "logger.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "variable.h"
#include "sceneobject.h"
#include "openGLES.h"

namespace RenderingEngine
{
    using std::vector;
    using std::unique_ptr;
    using std::make_unique;
    using std::make_shared;
    using std::string;
    using std::ifstream;
    using std::ios;
    using std::stringstream;
    using std::tuple;
    using std::make_tuple;
    using std::get;
    using glm::vec3;
    using glm::mat4;
    using glm::value_ptr;

    class SceneManager
    {
    private:
        // Vector with the objects that will be rendered
        vector<unique_ptr<SceneObject>> sceneobjects;

        // Attributes used on the shaders
        vector<unique_ptr<Variable>> attributes;

        // Uniforms used on the shaders
        vector<unique_ptr<Variable>> uniforms;

        // Projection matrix
        mat4 projectionMatrix;

        vec3 camera;
        float angle;

    public:
        SceneManager(ESContext*);
        ~SceneManager() = default;
        
        // Cleanup method
        void Clean();

        // Draw objects on screen
        void Draw();
        // Update the state of the objects
        void Update(ESContext*);

        // Handling camera
        vec3 getCamera() { return camera; }
        void setCamera(const vec3 &camera) { this->camera = camera; }

        // Modify the rotation angle of the objects
        float getAngle() { return angle; }
        void setAngle(const float &angle) { this->angle = angle; }
    };
}
