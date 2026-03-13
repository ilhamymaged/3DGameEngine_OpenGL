#include <glad/glad.h>
#include <Renderer/Grid.hpp>
#include <Renderer/ShaderLibrary.hpp>

namespace Agina
{
    Grid::Grid()
    {
        float vertices[] =
        {
            -1000.0f, 0.0f, -1000.0f,
             1000.0f, 0.0f, -1000.0f,
             1000.0f, 0.0f,  1000.0f,

            -1000.0f, 0.0f, -1000.0f,
             1000.0f, 0.0f,  1000.0f,
            -1000.0f, 0.0f,  1000.0f
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    Grid::~Grid()
    {
        glDeleteBuffers(1, &m_VBO);
        glDeleteVertexArrays(1, &m_VAO);
    }

    void Grid::Draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos)
    {
        auto shader = ShaderLibrary::Get("grid");
        shader->Use();

        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setVec3("cameraPos", cameraPos);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}