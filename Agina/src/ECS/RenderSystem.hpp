#pragma once
#include <entt/entt.hpp>

namespace Agina {
    class Camera;

    class RenderSystem 
    {
    public:
        static void Render(entt::registry& registry);
    };
}
