#include "Scene.hpp"
#include <ECS/Entity.hpp>
#include <ECS/Components.hpp>

namespace Agina
{
    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TagComponent>(name);
        entity.AddComponent<Transform>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    Scene* Scene::Copy(Scene* other)
    {
        Scene* newScene = new Scene();

        auto& srcRegistry = other->m_Registry;
        auto& dstRegistry = newScene->m_Registry;

        auto view = srcRegistry.view<TagComponent>(); 
        for (auto e : view)
        {
            Entity srcEntity = { e, other };
            auto& tag = srcEntity.GetComponent<TagComponent>().tag;

            Entity dstEntity = newScene->CreateEntity(tag);

            if (srcEntity.HasComponent<Transform>())
                dstEntity.AddComponent<Transform>(srcEntity.GetComponent<Transform>());

            if (srcEntity.HasComponent<MeshRenderer>())
                dstEntity.AddComponent<MeshRenderer>(srcEntity.GetComponent<MeshRenderer>());

            if (srcEntity.HasComponent<CameraComponent>())
                dstEntity.AddComponent<CameraComponent>(srcEntity.GetComponent<CameraComponent>());

            if (srcEntity.HasComponent<GridComponent>())
                dstEntity.AddComponent<GridComponent>(srcEntity.GetComponent<GridComponent>());

            if (srcEntity.HasComponent<SkyBoxComponent>())
                dstEntity.AddComponent<SkyBoxComponent>(srcEntity.GetComponent<SkyBoxComponent>());

        }

        return newScene;
    }

}