#pragma once

#include <entt/entt.hpp>

namespace Agina
{
    class Entity;
    class Scene
    {
    public:
        Entity CreateEntity(const std::string& name = "Empty Entity");
        void DestroyEntity(Entity entity);

        static Scene* Copy(Scene* other);

        entt::registry m_Registry;
        friend class Entity;
    };
}

