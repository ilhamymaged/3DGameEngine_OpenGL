#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <render/shader.hpp>
#include <iostream>
#include <memory>
#include <render/OpenGLObjects.hpp>
#include <scene/scene.hpp>
#include <core/layer.hpp>
#include <core/inputs.hpp>

class Renderer : public Layer
{
public:
    Renderer(GLFWwindow* window, int width, int height, Input& i);
    ~Renderer() = default;

    void Clear() const;
    void changeColor(float r, float g, float b) const;
    void Render();
    void Update(float dt);

    void onEvent(std::shared_ptr<Event> e) override;

private:
    GLFWwindow* window;
    std::shared_ptr<Shader> shader;
    std::unique_ptr<SceneManager> sceneManager;
};