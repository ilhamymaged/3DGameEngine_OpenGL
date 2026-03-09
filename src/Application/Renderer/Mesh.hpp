#pragma once

#include <Application/Renderer/OpenGLObjects.hpp>
#include <Application/Renderer/Vertex.hpp>    
#include <vector>
#include <iostream>
#include <memory>
#include <Application/Renderer/Texture.hpp>
#include <Application/Renderer/Shader.hpp>

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
        const std::vector<std::shared_ptr<Texture>>& textures);
    ~Mesh() = default;

    void Draw();  
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;
    std::shared_ptr<VAO> Vao;
    std::shared_ptr<VBO> Vbo;
    std::shared_ptr<EBO> Ebo;
};
