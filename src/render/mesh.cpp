#include <render/mesh.hpp>
#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
    const std::vector<std::shared_ptr<Texture>>& textures) :
    vertices(vertices), indices(indices), textures(textures)
{
    Vao = std::make_shared<VAO>();
    Vbo = std::make_shared<VBO>(vertices.data(), vertices.size() * sizeof(Vertex));
    Ebo = std::make_shared<EBO>(indices.data(), indices.size() * sizeof(unsigned int));

    Vao->Bind();
    Vbo->Bind();
    Ebo->Bind();

    Vao->AddAttribute(0, 3, sizeof(Vertex), (void*)offsetof(Vertex, m_position));
    Vao->AddAttribute(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
    Vao->AddAttribute(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));

    Vao->Unbind();
    Vbo->Unbind();
    Ebo->Unbind();
}

void Mesh::Draw()
{
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i]->Bind();
    }
    Vao->Bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    Vao->Unbind();
}
