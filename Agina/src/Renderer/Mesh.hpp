#pragma once

#include <Renderer/OpenGLObjects.hpp>
#include <Renderer/Vertex.hpp>    
#include <vector>
#include <iostream>
#include <memory>
#include <Renderer/Texture.hpp>
#include <Renderer/Shader.hpp>
#include <Renderer/Material.hpp>

namespace Agina
{
    struct SceneData;
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices,
            std::shared_ptr<Material> material);
        ~Mesh() = default;

        void Draw(const SceneData& sceneData, const glm::mat4& model);
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<Material> m_Material;
        std::shared_ptr<VAO> Vao;
        std::shared_ptr<VBO> Vbo;
        std::shared_ptr<EBO> Ebo;
    };
}