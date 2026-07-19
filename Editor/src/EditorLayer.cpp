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

#include <Core/FileSystem.hpp>

using namespace Agina;

void EditorLayer::OnAttach()
{
	std::string assetPath = (FileSystem::AppAssets()).string();
	m_Panel.SetContext(&m_Scene);

	UI::LoadFont(0, assetPath + "/fonts/Open_Sans/static/OpenSans-SemiBold.ttf", 16.0f, true);
	UI::LoadFont(1, assetPath + "/fonts/Open_Sans/static/OpenSans-Bold.ttf", 20.0f);
	UI::SetDarkEngineTheme();

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
	Vec2 viewPortSize = UI::Viewport(m_Framebuffer);
	m_ViewPortWidth = viewPortSize.x;
	m_ViewPortHeight = viewPortSize.y;
	UI::EndDockspace();
}

void EditorLayer::OnRender()
{
	const auto& spec = m_Framebuffer->GetSpecification();
	if (m_ViewPortWidth > 0 && m_ViewPortHeight > 0 &&
		(spec.Width != m_ViewPortWidth || spec.Height != m_ViewPortHeight))
	{
		m_Framebuffer->Resize(m_ViewPortWidth, m_ViewPortHeight);
	}

	RenderSystem::RenderToTarget(m_Scene, m_Framebuffer);
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

EditorLayer::EditorLayer(int width, int height)
	:m_ViewPortWidth(width), m_ViewPortHeight(height)
{
	FramebufferSpecification spec{ m_ViewPortWidth, m_ViewPortHeight };
	m_Framebuffer = std::make_Ref<Framebuffer>(spec);
}

void EditorLayer::OnDetach() {}
