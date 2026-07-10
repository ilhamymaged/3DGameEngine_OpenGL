#include "Entity.hpp"

namespace Agina {
    Entity::Entity(entt::entity h, entt::registry* reg) : m_Handle(h), m_Registry(reg) {}
}
