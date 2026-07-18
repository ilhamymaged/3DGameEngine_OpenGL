#pragma once

#include <entt/entt.hpp>

#include <Core/UUID.hpp>
#include "Components.hpp"

namespace Agina {

    class Entity
	{
    public:
        Entity() = default;
        Entity(entt::entity h, entt::registry* reg);

        inline entt::entity GetHandle() { return m_Handle; }

        bool operator==(const Entity& other) const
        {
            return m_Handle == other.m_Handle &&
                m_Registry == other.m_Registry;
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            return m_Registry->emplace<T>(m_Handle, std::forward<Args>(args)...);
        }
        template<typename T>
        T& GetComponent()
        {
            return m_Registry->get<T>(m_Handle);
        }

        UUID GetUUID() { return GetComponent<IDComponent>().ID; }

        operator bool() const { return m_Handle != entt::null; }

        template<typename T>
        bool HasComponent() const
        {
            return m_Registry->all_of<T>(m_Handle);
        }

        template<typename T>
        void RemoveComponent()
        {
            if (HasComponent<T>()) m_Registry->remove<T>(m_Handle);
        }

		inline bool IsValid() const { return m_Registry && 
			m_Registry->valid(m_Handle); }

	private:
		entt::entity m_Handle = {entt::null};
		entt::registry* m_Registry = nullptr;
	};


}

