#include <Layers/GameLayer/GameLayer.hpp>

void GameLayer::OnAttach()
{
	m_Camera = std::make_unique<Camera>();

	std::vector<Scene> scenes;

	//Creating The Scenes
	//...............
	m_SceneManager = std::make_unique<SceneManager>(scenes);
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(float dt)
{
	m_SceneManager->OnUpdate(dt);
	m_Camera->OnUpdate(dt);
}

void GameLayer::OnEvent(Event& e)
{
	m_SceneManager->OnEvent(e);
	m_Camera->OnEvent(e);
}
