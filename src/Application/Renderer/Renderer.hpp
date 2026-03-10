#pragma once

#include <glad/glad.h>
#include <Layers/GameLayer/Scene/Scene.hpp>
#include <Layers/GameLayer/Camera/Camera.hpp>

class Renderer 
{
public:
    Renderer();
    ~Renderer() = default;

    void Clear() const;
    void ChangeColor(float r, float g, float b) const;
    void RenderScene(Scene& scene, Camera& camera);

private:
    void RenderEntity(Entity& e, glm::mat4& view, glm::mat4& proj,
        glm::mat4& model);
};