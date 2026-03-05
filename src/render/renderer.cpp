#include <render/renderer.hpp>
#include <iostream>

Renderer::Renderer(GLFWwindow* Window, int width, int height, Input& i)
    : window(Window)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("GLAD initialization failed");
    }
        
    glEnable(GL_DEPTH_TEST);

    sceneManager = std::make_unique<SceneManager>(width, height, i);
    shader = std::make_shared<Shader>("../../../shaders/basic.vert", "../../../shaders/basic.fragment");
    shader->setInt("diffuseTexture", 0);
}

void Renderer::onEvent(std::shared_ptr<Event> e)
{
    sceneManager->onEvent(e);
}

void Renderer::Update(float dt)
{
    sceneManager->Update(dt);
}


void Renderer::changeColor(float r, float g, float b) const
{
    glClearColor(r, g, b, 1.0f);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render()
{
    Clear();

    shader->Use();

    Camera& camera = sceneManager->GetCamera();
    World& world = sceneManager->GetWorld();

    shader->setMat4("view", camera.GetViewMatrix());
    shader->setMat4("proj", camera.GetProjectionMatrix());

    for (auto& e : world.GetEntities())
        e.Draw(shader);
}