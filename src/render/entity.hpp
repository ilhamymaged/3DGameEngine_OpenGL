#pragma once

#include <render/model.hpp>
#include <render/transform.hpp>
#include <render/shader.hpp>
#include <event/events.hpp>

class Entity
{
public:
    Entity(std::shared_ptr<Model> model, const Transform& transform);

    void Draw(std::shared_ptr<Shader> shader) const;
    inline const glm::vec3& GetPosition() const { return transform.position; }
protected:  
    std::shared_ptr<Model> model;
    Transform transform;
};
