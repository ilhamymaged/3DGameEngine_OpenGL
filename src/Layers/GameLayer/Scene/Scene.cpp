#include <Layers/GameLayer/Scene/Scene.hpp>

Scene::Scene(std::vector<Entity> entities)
    : m_Entities(std::move(entities)) 
{
    m_SkyBox = nullptr;
}

Scene::Scene(std::vector<Entity> entities, std::unique_ptr<Grid> grid)
    :m_Entities(std::move(entities)), m_Grid(std::move(grid)){}

Scene::Scene(std::vector<Entity> entities, std::unique_ptr<SkyBox> skybox)
    :m_Entities(std::move(entities)), m_SkyBox(std::move(skybox)){}

Scene::Scene(std::vector<Entity> entities, std::unique_ptr<SkyBox> skybox, std::unique_ptr<Grid> grid)
    :m_Entities(std::move(entities)), m_SkyBox(std::move(skybox)), m_Grid(std::move(grid))
{}

Entity& Scene::AddEntity(const Entity& entity)
{
    m_Entities.push_back(entity);
    Entity& e = m_Entities.back();
    
    if (OnEntityAdded)
        OnEntityAdded(e);
    return e;
}

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


