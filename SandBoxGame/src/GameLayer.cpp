#include "GameLayer.hpp"

#include <ECS/PhysicsSystem.hpp>
#include <ECS/RenderSystem.hpp>
#include <ECS/Components.hpp>

#include <Application.hpp>
#include <Core/Inputs/KeyMappings.hpp>

using namespace Agina;
void GameLayer::OnAttach()
{		
	m_Scenes.reserve(10);
	m_Scenes.emplace_back(std::make_unique<Scene>());
	Scene& m_ActiveScene = *m_Scenes.at(m_ActiveSceneIndex);
	m_ActiveScene.OnCreate();

	m_Camera = m_ActiveScene.GetRegistry().create();
	auto& camComp = m_ActiveScene.GetRegistry().emplace<CameraComponent>(m_Camera);
	camComp.Cam = Camera(glm::vec3(0.0f, 2.0f, 5.0f));
	camComp.IsPrimary = true;
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher eventDispatcher(e);
	eventDispatcher.Dispatch<KeyPressed>([&](KeyPressed& key) 
	{
		if (key.getKey() == static_cast<int>(Key::Escape)) Application::Get().ShutDown();
	});

	Scene& m_ActiveScene = *m_Scenes.at(m_ActiveSceneIndex);
	auto& camComp = m_ActiveScene.GetRegistry().get<CameraComponent>(m_Camera);
	camComp.Cam.OnEvent(e);
}

void GameLayer::OnUpdate(float dt)
{
	Scene& m_ActiveScene = *m_Scenes.at(m_ActiveSceneIndex);
	m_ActiveScene.GetRegistry().get<CameraComponent>(m_Camera).Cam.Update(dt);
	PhysicsSystem::Update(m_ActiveScene.GetRegistry(), dt);
}

void GameLayer::OnRender()
{
	Scene& m_ActiveScene = *m_Scenes.at(m_ActiveSceneIndex);
	RenderSystem::Render(m_ActiveScene.GetRegistry());
}

void GameLayer::OnDetach() 
{
	Scene& m_ActiveScene = *m_Scenes.at(m_ActiveSceneIndex);
	m_ActiveScene.OnDestroy();
}

void GameLayer::OnUIRender() {}
