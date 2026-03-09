#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Application/Renderer/Shader.hpp>
#include <iostream>
#include <memory>
#include <Application/Renderer/OpenGLObjects.hpp>

class Renderer 
{
public:
    Renderer(GLFWwindow* window, int width, int height);
    ~Renderer() = default;

    void Clear() const;
    void changeColor(float r, float g, float b) const;
    void Render();

private:
    GLFWwindow* window;
    std::shared_ptr<Shader> shader;
};