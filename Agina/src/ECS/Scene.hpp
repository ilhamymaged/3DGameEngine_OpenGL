#pragma once

#include <entt/entt.hpp>
#include "Entity.hpp"
#include <optional>

namespace Agina {

    class UUID;
	class Scene
	{
    private:
        entt::registry m_Registry;

    public:
        Entity CreateEntity(const std::string& name = "");
        void DestroyEntity(const Entity& entity);
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "");

        template<typename... Components>
        std::optional<Entity> FindEntityWithComponent()
        {
            auto view = m_Registry.view<Components...>();
            if (!view.empty())
                return Entity{ view.front(), &m_Registry };
            return std::nullopt;
        }

        template<typename... Components, typename Func>
        void Each(Func&& func)
        {
            auto view = m_Registry.template view<Components...>();
            for (auto entity : view)
            {
                func(Entity{ entity, &m_Registry });
            }
        }

        template<typename Func>
        void EachEntity(Func&& func)
        {
            auto& view = m_Registry.storage<entt::entity>();
            for (auto entity : view)
            {
                func(Entity{ entity, &m_Registry });
            }
        }
	};

}
