#pragma once

#include <entt/entt.hpp>

namespace Agina {

    class Entity
	{
    public:
        Entity(entt::entity h, entt::registry* reg);

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

		inline bool IsValid() const { return m_Registry && 
			m_Registry->valid(m_Handle); }

	private:
		entt::entity m_Handle = {entt::null};
		entt::registry* m_Registry = nullptr;
	};


}

