#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderBackend.hpp"
#include <Core/Logger/Logger.hpp>

namespace Agina {

    std::unique_ptr<Mesh> RenderBackend::s_Mesh = nullptr;

    void RenderBackend::Init()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            AG_CORE_ERROR("Failed To Load Glad");
            throw std::runtime_error("Failed To Load Glad");
        }

        InitQuad();
    }

    void RenderBackend::Shutdown()
    {
    }

    void RenderBackend::DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, Texture& texture)
    {
        texture.Bind();
        glBindVertexArray(s_Mesh->GetVAO());
        glDrawElements(GL_TRIANGLES, s_Mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    void RenderBackend::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void RenderBackend::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderBackend::InitQuad()
    {
        std::vector<Agina::Vertex> vertices = 
        {
            {{-0.5f,  0.5f, 0.0f},   {0.0f, 0.0f, 1.0f},    {0.0f, 1.0f}}, 
            {{-0.5f, -0.5f, 0.0f},   {0.0f, 0.0f, 1.0f},    {0.0f, 0.0f}}, 
            {{ 0.5f, -0.5f, 0.0f},   {0.0f, 0.0f, 1.0f},    {1.0f, 0.0f}}, 
            {{ 0.5f,  0.5f, 0.0f},   {0.0f, 0.0f, 1.0f},    {1.0f, 1.0f}}  
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2, 
            0, 2, 3  
        };

        s_Mesh = std::make_unique<Mesh>(vertices, indices);
    }
}
