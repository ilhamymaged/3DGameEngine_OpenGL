#include "EditorLayer.hpp"
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

using namespace Agina;

void EditorLayer::OnAttach()
{
	m_Panel.SetContext(&m_Scene);

	auto camera = m_Scene.CreateEntity();
	auto& camereComp = camera.AddComponent<CameraComponent>();
	camera.AddComponent<TagComponent>("Camera");
	camera.AddComponent<Transform>(camereComp.Cam.GetRefPos());

	auto sphere = m_Scene.CreateEntity();
	sphere.AddComponent<TagComponent>("Sphere");
	sphere.AddComponent<Transform>(Vec3(5.0, 4.0f, 0.0));
	auto& sMat = sphere.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::SPHERE),
		Material::Create(MaterialType::LIT));
	sMat.MaterialAsset->Set("u_HasColor", true);
	sMat.MaterialAsset->Set("u_Color", Vec3(0.118f, 0.69f, 0.655f));
	sMat.MaterialAsset->Set("u_HasAlbedoTexture", false);
	sMat.MaterialAsset->Set("u_HasNormalTexture", false);
	sMat.MaterialAsset->Set("u_HasSpecularTexture", false);

	auto cube = m_Scene.CreateEntity();
	cube.AddComponent<TagComponent>("Cube");
	cube.AddComponent<Transform>(Vec3(2.0, 4.0f, 0.0));
	auto& cMat = cube.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::CUBE),
		Material::Create(MaterialType::LIT));
	cMat.MaterialAsset->Set("u_HasColor", true);
	cMat.MaterialAsset->Set("u_Color", Vec3(0.212f, 0.271f, 0.741f));
	cMat.MaterialAsset->Set("u_HasAlbedoTexture", false);
	cMat.MaterialAsset->Set("u_HasNormalTexture", false);
	cMat.MaterialAsset->Set("u_HasSpecularTexture", false);

	auto terrain = m_Scene.CreateEntity();
	terrain.AddComponent<TagComponent>("Terrain");
	auto& tMat = terrain.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::TERRAIN),
		Material::Create(MaterialType::LIT));
	tMat.MaterialAsset->Set("u_HasColor", true);
	tMat.MaterialAsset->Set("u_Color", Vec3(0.071f, 0.722f, 0.255f));
	tMat.MaterialAsset->Set("u_HasAlbedoTexture", false);
	tMat.MaterialAsset->Set("u_HasNormalTexture", false);
	tMat.MaterialAsset->Set("u_HasSpecularTexture", false);

	auto skyBox = m_Scene.CreateEntity();
	skyBox.AddComponent<SkyboxComponent>(std::make_shared<Skybox>(),
		Material::Create(MaterialType::SKYBOX));
	skyBox.AddComponent<TagComponent>("SkyBox");

}

void EditorLayer::OnUIRender()
{
	UI::BeginDockspace();
	m_Panel.OnUIRender();
	UI::Viewport(m_Framebuffer);
	UI::EndDockspace();
}

void EditorLayer::OnRender()
{
	RenderSystem::RenderToTarget(m_Scene, m_Framebuffer);
}

void EditorLayer::OnEvent(Agina::Event& e)
{
	Agina::EventDispatcher eventDispatcher(e);
	eventDispatcher.Dispatch<Agina::KeyPressed>([&](Agina::KeyPressed& key)
		{
			if (key.getKey() == static_cast<int>(Agina::Key::Escape)) Agina::Application::Get().ShutDown();
		});

	eventDispatcher.Dispatch<Agina::WindowResized>([&](Agina::WindowResized& wr)
		{
			wr.GetNewWidth(), wr.GetNewHeight();
		});

	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.OnEvent(e);
}

void EditorLayer::OnUpdate(float dt)
{
	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.Update(dt);
}

EditorLayer::EditorLayer(int width, int height)
	:m_Width(width), m_Height(height)
{
	FramebufferSpecification spec{ m_Width, m_Height };
	m_Framebuffer = std::make_shared<Framebuffer>(spec);
}

void EditorLayer::OnDetach() {}
