#pragma once

#include <render/OpenGLObjects.hpp>
#include <render/vertex.hpp>    
#include <vector>
#include <iostream>
#include <memory>
#include <render/texture.hpp>
#include <render/shader.hpp>

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
