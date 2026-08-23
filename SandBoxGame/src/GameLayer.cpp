#include "GameLayer.hpp"

#include <ECS/PhysicsSystem.hpp>
#include <ECS/RenderSystem.hpp>
#include <ECS/Components.hpp>

#include <Application.hpp>
#include <Core/KeyMappings.hpp>

#include <Core/AssetManager.hpp>
#include <Renderer/Material.hpp>

#include <Core/MathTypes.hpp>

#include <UI/UI.hpp>

#include <Core/Logger.hpp>

#include <Core/FileSystem.hpp>

using namespace Agina;
void GameLayer::OnAttach()
{	
	const std::string assetPath = (FileSystem::AppAssets()).string();

	auto camera = m_Scene.CreateEntity();
	camera.AddComponent<CameraComponent>();
	camera.AddComponent<TagComponent>("Camera");
}

void GameLayer::OnEvent(Event& e)
{
	::EventDispatcher eventDispatcher(e);
	eventDispatcher.Dispatch<KeyPressed>([&](KeyPressed& key) 
	{
		if (key.getKey() == static_cast<int>(Key::Escape)) Application::Get().ShutDown();
	});

	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.OnEvent(e);
}

void GameLayer::OnUpdate(float dt)
{
	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.Update(dt);
}

void GameLayer::OnRender()
{
	//RenderSystem::Render(m_Scene);	
}

void GameLayer::OnDetach() 
{
}

void GameLayer::OnUIRender() 
{
}

