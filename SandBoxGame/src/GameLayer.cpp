#include "GameLayer.hpp"

#include <ECS/PhysicsSystem.hpp>
#include <ECS/RenderSystem.hpp>
#include <ECS/Components.hpp>

#include <Application.hpp>
#include <Core/Inputs/KeyMappings.hpp>

#include <Core/AssetManager/AssetManager.hpp>
#include <Renderer/Material.hpp>

using namespace Agina;

void GameLayer::OnAttach()
{	
	//Camera
	Entity camera = m_Scene.CreateEntity();
	camera.AddComponent<CameraComponent>();

	//Sphere
	Entity sphere = m_Scene.CreateEntity();
	sphere.AddComponent<Transform>(glm::vec3(0.0f, 5.0f, 0.0f));
	auto& sMat = sphere.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::SPHERE), 
		Material::Create(MaterialType::LIT));
	sMat.MaterialAsset->Set("u_HasColor", true);
	sMat.MaterialAsset->Set("u_Color", glm::vec3(1.0f, 1.0f, 0.0f));

	//Floor 
	Entity ground = m_Scene.CreateEntity();
	ground.AddComponent<Transform>(glm::vec3(0.0f));
	auto& gMat = ground.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::TERRAIN),
		Material::Create(MaterialType::LIT));
	gMat.MaterialAsset->Set("u_HasColor", true);
	gMat.MaterialAsset->Set("u_Color", glm::vec3(0.0f, 1.0f, 0.0f));

	//SkyBox
	Entity skyBox = m_Scene.CreateEntity();
	skyBox.AddComponent<SkyboxComponent>(std::make_shared<Skybox>(), Material::Create(MaterialType::SKYBOX));
}

void GameLayer::OnEvent(Agina::Event& e)
{
	Agina::EventDispatcher eventDispatcher(e);
	eventDispatcher.Dispatch<Agina::KeyPressed>([&](Agina::KeyPressed& key) 
	{
		if (key.getKey() == static_cast<int>(Agina::Key::Escape)) Agina::Application::Get().ShutDown();
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
	RenderSystem::Render(m_Scene);
}

void GameLayer::OnDetach() 
{
}

void GameLayer::OnUIRender() {}
