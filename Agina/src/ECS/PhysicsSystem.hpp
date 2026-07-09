#pragma once
#include <entt/entt.hpp>
#include <Renderer/Mesh.hpp>

namespace Agina {
    class PhysicsSystem
    {
    public:
        static void Update(entt::registry& registry, float dt);
    };
}
