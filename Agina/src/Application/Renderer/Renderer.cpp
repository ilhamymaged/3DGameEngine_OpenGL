#include <Application/Renderer/Renderer.hpp>
#include <Application/Renderer/ShaderLibrary.hpp>
#include <GLFW/glfw3.h>
#include <Application/Renderer/OpenGLObjects.hpp>

void Renderer::Init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("GLAD initialization failed");
    }
        
    glEnable(GL_DEPTH_TEST);
    
    ShaderLibrary::Load("basic", "basic.vert", "basic.fragment");
    ShaderLibrary::Load("skybox", "skybox.vert", "skybox.fragment");
    ShaderLibrary::Load("grid", "grid.vert", "grid.fragment");
}

void Renderer::Clear(const glm::vec4& color) 
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void Renderer::DrawIndexed(const std::shared_ptr<VAO>& vertexArray, uint32_t indexCount)
{
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    vertexArray->Unbind();
}
