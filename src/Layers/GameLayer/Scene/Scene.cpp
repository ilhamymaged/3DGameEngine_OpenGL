#include <Layers/GameLayer/Scene/Scene.hpp>

Scene::Scene(std::vector<Entity> entities): m_Entities(std::move(entities)) {}

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


