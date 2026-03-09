#include <Layers/GameLayer/Scene/SceneManager.hpp>

SceneManager::SceneManager(const std::vector<Scene>& scenes) : m_Scenes{scenes} {}

void SceneManager::OnUpdate(float dt)
{
	for (auto& scene : m_Scenes) scene.OnUpdate(dt);
}

void SceneManager::OnEvent(Event& e)
{
	for (auto& scene : m_Scenes) scene.OnEvent(e);
}