#pragma once

// STD
#include <vector>
#include <memory>

// GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Engine
#include "variable.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"

namespace RenderingEngine
{
    using std::vector;
    using std::unique_ptr;
    using std::shared_ptr;
    using std::move;
    using glm::vec3;
    using glm::mat4;
    using glm::scale;
    using glm::translate;
    using glm::rotate;

    class SceneObject
    {
    private:
        shared_ptr<Shader> shader;
        vector<unique_ptr<Variable>> uniforms;
        unique_ptr<Mesh> mesh;
        unique_ptr<Texture> texture;

        // Transformations
        vec3 position;
        vec3 scaling;
        vec3 rotation;
        float angle;

        // Projection uniform
        GLint projectionUni;
        // Modelview uniform
        GLint modelviewUni;

        // Model matrix
        mat4 modelMatrix;
        // Modelview matrix
        mat4 modelviewMatrix;

        bool skymap;

    public:
        SceneObject() : angle{ 0.0f }, projectionUni{ -1 }, modelviewUni{ -1 }, skymap{ false } {}
        ~SceneObject()
        {
            Log << Function << endl;
            uniforms.clear();
        }

        // Set and get the shader of the object
        void SetShader(shared_ptr<Shader> shader) { this->shader = move(shader); }
        shared_ptr<Shader> &GetShader() { return shader; }

        // Set and get the mesh of the object
        void SetMesh(unique_ptr<Mesh> mesh) { this->mesh = move(mesh); }
        unique_ptr<Mesh> &GetMesh() { return mesh; }

        // Set and get the texture of the object
        void SetTexture(unique_ptr<Texture> texture) { this->texture = move(texture); }
        unique_ptr<Texture> &GetTexture() { return texture; }

        // Set the coordinates of the object (in model space)
        void SetCoordinates(const vec3 &pos, const vec3 &scl, const vec3 &rot, float ang) 
        {
            position = pos; scaling = scl; rotation = rot; angle = ang;
            if (angle != 0)
                modelMatrix = translate(position) * scale(mat4(1.0f), scaling) * rotate(mat4(1.0f), angle, rotation);
            else
                modelMatrix = translate(position) * scale(mat4(1.0f), scaling);
        }

        // Set and get the projection uniform
        void SetProjectionUni(const GLint &projectionUni) { this->projectionUni = projectionUni; }
        GLint GetProjectionUni() { return projectionUni; }

        // Set and get the modelview uniform
        void SetModelviewUni(const GLint &modelviewUni) { this->modelviewUni = modelviewUni; }
        GLint GetModelviewUni() { return modelviewUni; }

        void updateModelview(const vec3 &camera, const float &keyangle)
        {
            // Apply the modifications done in the application with the movement keys
            modelviewMatrix = translate(mat4(1.0f), camera) * rotate(mat4(1.0f), keyangle, vec3(0.0f, 1.0f, 0.0f)) * modelMatrix;
        }
        mat4 &GetModelviewMatrix() { return modelviewMatrix; }

        // Define if the object is a skymap in order to render it correctly
        void SetSkymap() { this->skymap = true; }
        bool IsSkymap() { return skymap; }
    };
}
