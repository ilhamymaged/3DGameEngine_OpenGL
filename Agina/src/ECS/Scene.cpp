#include "Scene.hpp"
#include <vector>
#include <ECS/Components.hpp>
#include <Core/UUID.hpp>

namespace Agina {

    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithUUID(UUID(), name);
    }

    void Scene::DestroyEntity(const Entity& entity)
    {
        m_Registry.destroy(entity.GetHandle());
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string & name)
    {
        Entity entity = { m_Registry.create(), &m_Registry };
        entity.AddComponent<Transform>();
        entity.AddComponent<IDComponent>(uuid);
        auto& tag = entity.AddComponent<TagComponent>().tag;
        tag = name.empty() ? "Entity" : name;
        return entity;
    }

}

