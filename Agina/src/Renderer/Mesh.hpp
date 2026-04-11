#pragma once

#include <Renderer/OpenGLObjects.hpp>
#include <Renderer/Vertex.hpp>    
#include <vector>
#include <iostream>
#include <memory>
#include <Renderer/Material.hpp>

namespace Agina
{
    struct SceneData;
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices);
        ~Mesh() = default;

        void Draw();
    private:
        VAO m_VAO;
        VBO m_VBO;
        EBO m_EBO;

		uint32_t m_IndexCount;
    };
}