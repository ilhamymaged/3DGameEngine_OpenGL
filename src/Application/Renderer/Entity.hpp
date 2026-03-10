#pragma once

#include <Application/Renderer/Model.hpp>
#include <Application/Renderer/Transform.hpp>
#include <Application/Renderer/Shader.hpp>

class Entity
{
public:
    Entity(std::shared_ptr<Model> model, const Transform& transform);

    inline glm::mat4& GetModelMatrix() { return m_Transform.GetModelMatrix(); }
    
    inline Model& GetModel() { return *m_Model; }

protected:  
    std::shared_ptr<Model> m_Model;
    Transform m_Transform;
};
