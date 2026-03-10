#pragma once

#include <Application/Renderer/OpenGLObjects.hpp>
#include <Application/Renderer/Vertex.hpp>    
#include <vector>
#include <iostream>
#include <memory>
#include <Application/Renderer/Texture.hpp>
#include <Application/Renderer/Shader.hpp>
#include <Application/Renderer/Material.hpp>
#include <Application/Renderer/SceneRenderer.hpp>

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, 
        const std::vector<unsigned int>& indices,
        std::shared_ptr<Material> material);
    ~Mesh() = default;

    void Draw(const SceneData& sceneData,const glm::mat4& model);  
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<Material> m_Material;
    std::shared_ptr<VAO> Vao;
    std::shared_ptr<VBO> Vbo;
    std::shared_ptr<EBO> Ebo;
};
