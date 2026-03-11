#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

class VAO;
class Renderer
{
public:
    static void Init();
    static void Clear(const glm::vec4& color = { 0.1f, 0.1f, 0.1f, 1.0f });
    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void DrawIndexed(const std::shared_ptr<VAO>& vertexArray, uint32_t indexCount = 0);
};