#pragma once
#include <Ecs/Scene.hpp>

namespace Agina {
    class PhysicsSystem
    {
    public:
        static void Update(Scene& scene, float dt);
    };
}
