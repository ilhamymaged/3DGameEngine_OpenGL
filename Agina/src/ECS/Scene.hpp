#pragma once

#include <entt/entt.hpp>
#include "Entity.hpp"
#include <optional>

namespace Agina {

	class Scene
	{
    private:
        entt::registry m_Registry;

    public:
        Entity CreateEntity();

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
	};

}
