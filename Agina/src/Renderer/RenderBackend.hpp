#pragma once

#include <glm/glm.hpp>
#include "Texture.hpp"
#include <Renderer/Mesh.hpp>

namespace Agina {

    class RenderBackend
    {
    public:
        static void Init();
        static void Shutdown();

        static void DrawQuad(const glm::vec2& pos,
                           const glm::vec2& size,
                           Texture& texture);

        static void SetClearColor(float r, float g, float b, float a);
        static void Clear();

    private:
        static void InitQuad();

    private:
        static std::unique_ptr<Mesh> s_Mesh;
    };

}