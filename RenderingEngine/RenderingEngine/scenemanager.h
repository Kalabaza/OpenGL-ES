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
    using std::string;
    using std::ifstream;
    using std::ios;
    using std::stringstream;
    using std::tuple;
    using std::make_tuple;
    using std::get;
    using glm::vec3;
    using glm::mat4;

    class SceneManager
    {
    private:
        // Shaders will be held in here
        vector<unique_ptr<Shader>> shaders;
        // Object files go in here
        vector<unique_ptr<Mesh>> meshes;
        // Textures are saved in here
        vector<unique_ptr<Texture>> textures;
        // Properties of the scene objects
        vector<unique_ptr<SceneObject>> sceneobjects;

        GLuint projectionUniform;
        GLuint modelviewUniform;

        // Temporary vectors to hold the parameters of the resources.txt file
        vector<tuple<string, string>> shader;
        vector<tuple<string, unsigned int>> mesh;
        vector<tuple<string, unsigned int>> texture;
        vector<vec3> position;
        vector<vec3> scale;
        vector<vec3> rotation;

        string projectionName;
        string modelviewName;

        mat4 projection;
        vec3 camera;
        float angle;

        // Attributes used on the shaders
        vector<unique_ptr<Variable>> attributes;

        // Uniforms used on the shaders
        vector<unique_ptr<Variable>> uniforms;

    public:
        SceneManager(ESContext*);
        ~SceneManager();

        void draw();
        void update(ESContext*);

        vec3 getCamera() { return camera; }
        void setCamera(const vec3 &camera) { this->camera = camera; }

        float getAngle() { return angle; }
        void setAngle(const float &angle) { this->angle = angle; }
    };
}
