#include <Renderer/Mesh.hpp>

namespace Agina
{
    Mesh::Mesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices)
        :m_IndexCount(indices.size()), m_VAO(),
		m_VBO(vertices.data(), vertices.size() * sizeof(Vertex)),
		m_EBO(indices.data(), indices.size() * sizeof(unsigned int))
    {

        m_VAO.Bind();
        m_VBO.Bind();
        m_EBO.Bind();

        m_VAO.AddAttribute(0, 3, sizeof(Vertex), (void*)offsetof(Vertex, m_position));
        m_VAO.AddAttribute(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
        m_VAO.AddAttribute(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));

        m_VAO.Unbind();
    }

    void Mesh::Draw()
    {
        m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
    }
}
