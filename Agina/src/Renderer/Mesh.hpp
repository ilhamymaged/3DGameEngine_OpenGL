#pragma once

#include <Renderer/OpenGLObjects.hpp>
#include <Renderer/Vertex.hpp>    
#include <vector>
#include <iostream>
#include <memory>

namespace Agina
{
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices);
        ~Mesh() = default;

        uint32_t GetVAO();
        uint32_t GetIndexCount();

    private:
        VAO m_VAO;
        VBO m_VBO;
        EBO m_EBO;

		uint32_t m_IndexCount;
    };
}