#pragma once

#include <Application/Renderer/OpenGLObjects.hpp>
#include <Application/Renderer/Vertex.hpp>    
#include <vector>
#include <iostream>
#include <memory>
#include <Application/Renderer/Texture.hpp>
#include <Application/Renderer/Shader.hpp>
#include <Application/Renderer/Material.hpp>

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, 
        const std::vector<unsigned int>& indices,
        std::shared_ptr<Material> material);
    ~Mesh() = default;

    void Draw(glm::mat4& view, glm::mat4& proj, glm::mat4& model);  
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<Material> m_Material;
    std::shared_ptr<VAO> Vao;
    std::shared_ptr<VBO> Vbo;
    std::shared_ptr<EBO> Ebo;
};
