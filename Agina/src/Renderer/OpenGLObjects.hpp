#pragma once

#include <glad/glad.h>

namespace Agina
{
    class VBO
    {
    public:
        VBO(const void* data, unsigned int size);
        ~VBO();

        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_ID;
    };

    class VAO
    {
    public:
        VAO();
        ~VAO();

        void AddAttribute(unsigned int index, int size,
            GLsizei stride, const void* pointer) const;
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_ID;
    };

    class EBO
    {
    public:
        EBO(const void* data, unsigned int size);
        ~EBO();

        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_ID;
    };
}