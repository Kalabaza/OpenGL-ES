#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderingEngine
{
    using glm::vec3;
    using glm::mat4;
    using glm::scale;
    using glm::translate;
    using glm::rotate;

    class SceneObject
    {
    private:
        vec3 position;
        vec3 scaling;
        vec3 rotation;
        float angle;

        mat4 model;
        mat4 modelview;

    public:
        SceneObject(const vec3 &position, const vec3 &scaling, const float &angle, const vec3 &rotation) : position{ position }, scaling{ scaling }, angle{ angle }, rotation{ rotation }
        {
            // First set the initial properties of the model, like its translation, scaling and rotation
            model = translate(position) * scale(mat4(), scaling) * rotate(mat4(), angle, rotation);
        }
        ~SceneObject() = default;

        void updateModelTransform(const vec3 &camera, const float &keyangle);

        mat4 &getModelTransform() { return modelview; }
    };
}
