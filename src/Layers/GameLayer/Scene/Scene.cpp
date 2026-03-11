#include <Layers/GameLayer/Scene/Scene.hpp>

Scene::Scene(entt::registry& e): m_Registry(std::move(e)){}

void Scene::OnUpdate(float dt){}
void Scene::OnEvent(Event& e){}
