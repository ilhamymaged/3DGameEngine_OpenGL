#include <Renderer/OpenGLObjects.hpp>

namespace Agina
{
    EBO::EBO(const void* data, unsigned int size)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    EBO::~EBO()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void EBO::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    void EBO::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void VAO::AddAttribute(unsigned int index, int size,
        GLsizei stride, const void* pointer) const
    {
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    VAO::VAO()
    {
        glGenVertexArrays(1, &m_ID);
    }

    VAO::~VAO()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VAO::Bind() const
    {
        glBindVertexArray(m_ID);
    }

    void VAO::Unbind() const
    {
        glBindVertexArray(0);
    }

    VBO::VBO(const void* data, unsigned int size)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VBO::~VBO()
    {
        glDeleteBuffers(1, &m_ID);
    }


    void VBO::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    void VBO::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}