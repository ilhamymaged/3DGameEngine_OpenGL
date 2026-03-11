#include <Application/Renderer/Mesh.hpp>
#include <Application/Renderer/Renderer.hpp>
#include <Layers/GameLayer/Systems/RenderSystem.hpp>

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

void Mesh::Draw(const SceneData& sceneData,const glm::mat4& model)
{
    m_Material->Bind();
    auto& shader = m_Material->GetShader();
    shader.setMat4("view", sceneData.ViewMatrix);
    shader.setMat4("proj", sceneData.ProjectionMatrix);

    shader.setMat4("model", model);
    Renderer::DrawIndexed(Vao, static_cast<uint32_t>(indices.size()));
}

