#pragma once

#include <Application/Renderer/Model.hpp>
#include <Application/Renderer/Transform.hpp>
#include <Application/Renderer/Shader.hpp>

class Entity
{
public:
    Entity(std::shared_ptr<Model> model, const Transform& transform);

    void Draw(Shader& shader) const;
    inline const glm::vec3& GetPosition() const { return transform.position; }
protected:  
    std::shared_ptr<Model> model;
    Transform transform;
};
