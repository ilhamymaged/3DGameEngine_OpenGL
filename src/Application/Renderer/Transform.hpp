#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{

    //Default constructor
    Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}
    Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
        :position(pos), rotation(rot), scale(scale){}

    glm::mat4 ModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0);

        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);

        return model;
    }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};
