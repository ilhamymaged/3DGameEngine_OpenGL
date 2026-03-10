#include <Application/Renderer/Renderer.hpp>
#include <Application/Renderer/ShaderLibrary.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

Renderer::Renderer()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("GLAD initialization failed");
    }
        
    glEnable(GL_DEPTH_TEST);

    ShaderLibrary::Load("basic", "../../../shaders/basic.vert",
        "../../../shaders/basic.fragment");
}

void Renderer::ChangeColor(float r, float g, float b) const
{
    glClearColor(r, g, b, 1.0f);
}

void Renderer::RenderScene(Scene& scene, Camera& camera)
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 proj = camera.GetProjectionMatrix();

    glm::vec3 lightPos(0.0f, 4.0f, -3.0f);
    glm::vec3 lightColor(1.0f);

    std::vector<Entity>& entities = scene.GetEntities();
    for (auto& e : entities)
        RenderEntity(e, view, proj, e.GetModelMatrix());
}

void Renderer::RenderEntity(Entity& e, glm::mat4& view, glm::mat4& proj,
    glm::mat4& model)
{
    Model& m_Model = e.GetModel();
    m_Model.Draw(view, proj, model);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

