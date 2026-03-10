#include <glad/glad.h>
#include <Application/Renderer/Mesh.hpp>
#include <Application/Renderer/ShaderLibrary.hpp>

Mesh::Mesh(const std::vector<Vertex>& vertices, 
    const std::vector<unsigned int>& indices,
    std::shared_ptr<Material> material)
    :vertices(vertices), indices(indices), m_Material(material)
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

void Mesh::Draw(glm::mat4& view, glm::mat4& proj, glm::mat4& model)
{
    m_Material->Bind();

    auto shader = m_Material->GetShader();

    shader.setMat4("view", view);
    shader.setMat4("proj", proj);
    shader.setMat4("model", model);

    Vao->Bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    Vao->Unbind();
}

