#include <Layers/GameLayer/Scene/Scene.hpp>

Scene::Scene(std::vector<Entity> entities)
    : m_Entities(std::move(entities)) 
{
    m_SkyBox = nullptr;
}

Scene::Scene(std::vector<Entity> entities, std::unique_ptr<SkyBox> skybox)
    :m_Entities(std::move(entities)), m_SkyBox(std::move(skybox)){}

std::vector<Entity>& Scene::GetEntities() 
{
    return m_Entities;
}

void Scene::OnUpdate(float dt)
{

}

void Scene::OnEvent(Event& e)
{
        
}


