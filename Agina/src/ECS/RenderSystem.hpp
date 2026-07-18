#pragma once
#include "Scene.hpp"
#include <Renderer/Framebuffer.hpp>

namespace Agina {
    class RenderSystem 
    {
    public:
        static void Render(Scene& scene, const std::shared_ptr<Framebuffer>& target);
        static void RenderToTarget(Scene& scene, const std::shared_ptr<Framebuffer>& target);
    };
}
