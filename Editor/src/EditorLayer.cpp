#include "EditorLayer.hpp"
#include <ECS/PhysicsSystem.hpp>
#include <ECS/RenderSystem.hpp>
#include <ECS/Components.hpp>

#include <Application.hpp>
#include <Core/Inputs/KeyMappings.hpp>

#include <Core/AssetManager/AssetManager.hpp>
#include <Renderer/Material.hpp>

#include <Core/Math/MathTypes.hpp>

#include <UI/UI.hpp>

#include <Core/Logger/Logger.hpp>

using namespace Agina;

void EditorLayer::OnAttach()
{
	Entity camera = m_Scene.CreateEntity();
	camera.AddComponent<CameraComponent>();

	Entity sphere = m_Scene.CreateEntity();
	auto sphereMesh = AssetManager::LoadMesh(MeshType::SPHERE);
	sphere.AddComponent<Transform>(Vec3(0.0f, 5.0f, 0.0f));
	auto& sMat = sphere.AddComponent<MeshComponent>(sphereMesh, Material::Create(MaterialType::LIT));
	sMat.MaterialAsset->Set("u_HasColor", true);
	sMat.MaterialAsset->Set("u_Color", Vec3(1.0f, 1.0f, 0.0f));

	Entity ground = m_Scene.CreateEntity();
	auto groundMesh = AssetManager::LoadMesh(MeshType::TERRAIN);
	ground.AddComponent<Transform>(Vec3(0.0f));
	auto& gMat = ground.AddComponent<MeshComponent>(groundMesh, Material::Create(MaterialType::LIT));
	gMat.MaterialAsset->Set("u_HasColor", true);
	gMat.MaterialAsset->Set("u_Color", Vec3(0.0f, 1.0f, 0.0f));

	Entity skyBox = m_Scene.CreateEntity();
	skyBox.AddComponent<SkyboxComponent>(std::make_shared<Skybox>(), Material::Create(MaterialType::SKYBOX));
}

void EditorLayer::OnUIRender()
{

}

void EditorLayer::OnRender()
{
	RenderSystem::Render(m_Scene);
}

void EditorLayer::OnEvent(Agina::Event& e)
{
	Agina::EventDispatcher eventDispatcher(e);
	eventDispatcher.Dispatch<Agina::KeyPressed>([&](Agina::KeyPressed& key)
		{
			if (key.getKey() == static_cast<int>(Agina::Key::Escape)) Agina::Application::Get().ShutDown();
		});

	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.OnEvent(e);
}

void EditorLayer::OnUpdate(float dt)
{
	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.Update(dt);
}

void EditorLayer::OnDetach() {}
