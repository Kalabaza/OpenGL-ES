#include "sceneobject.h"

namespace RenderingEngine
{
    void SceneObject::updateModelTransform(const vec3 &camera, const float &keyangle)
    {
        
        // Now apply the modifications done in the application with the movement keys
        modelview = translate(mat4(), camera) * rotate(mat4(), keyangle, vec3(0.0f, 1.0f, 0.0f)) * model;
    }
}
