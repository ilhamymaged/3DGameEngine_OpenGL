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

	auto light = m_Scene.CreateEntity();
	auto& dl = light.AddComponent<DirectionalLightComponent>(Vec3(0.0f));
	light.AddComponent<TagComponent>("Light");
	Vec3 lightPos(-10.0f, 20.0f, -5.0f);
	light.AddComponent<Transform>(Vec3(lightPos));

	auto camera = m_Scene.CreateEntity();
	auto& camereComp = camera.AddComponent<CameraComponent>();
	camera.AddComponent<TagComponent>("Camera");
	camera.AddComponent<Transform>(camereComp.Cam.GetRefPos());

	auto mesh = m_Scene.CreateEntity();
	mesh.AddComponent<MeshComponent>(AssetManager::LoadMesh(
		MeshType::CUBE),
		Material::Create(MaterialType::LIT)
	);

	Entity newGrid = m_Scene.CreateEntity("Grid");
	newGrid.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::GRID),
		Material::Create(MaterialType::GRID));
	{
		auto SkyBox = m_Scene.CreateEntity("SkyBox");
		std::vector<std::string> facePaths = {
			assetPath + "/skyboxes/sky1/" + "right.jpg",
			assetPath + "/skyboxes/sky1/" + "left.jpg",
			assetPath + "/skyboxes/sky1/" + "top.jpg",
			assetPath + "/skyboxes/sky1/" + "bottom.jpg",
			assetPath + "/skyboxes/sky1/" + "front.jpg",
			assetPath + "/skyboxes/sky1/" + "back.jpg",
		};
		std::Ref<CubemapTexture> c = std::make_Ref<CubemapTexture>(facePaths);
		SkyBox.AddComponent<SkyboxComponent>(std::make_Ref<Skybox>(c), 
			Material::Create(MaterialType::SKYBOX));
	}
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

	m_Framebuffer->Bind();
	m_Framebuffer->ClearAttachment(1, -1);
	m_Framebuffer->Unbind();

	RenderSystem::RenderToTarget(m_Scene, m_Framebuffer);
}

void EditorLayer::OnEvent(Agina::Event& e)
{
	Agina::EventDispatcher eventDispatcher(e);
	eventDispatcher.Dispatch<Agina::KeyPressed>([&](Agina::KeyPressed& key)
		{
			if (key.getKey() == static_cast<int>(Agina::Key::Escape)) Agina::Application::Get().ShutDown();
			if (key.getKey() == static_cast<int>(Agina::Key::R)) AssetManager::HotReloadShaders();
		});

	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.OnEvent(e);
}

void EditorLayer::OnUpdate(float dt)
{
	auto cameraEntity = m_Scene.FindEntityWithComponent<CameraComponent>();
	if (cameraEntity.has_value()) cameraEntity->GetComponent<CameraComponent>().Cam.Update(dt);
	PhysicsSystem::Update(m_Scene, dt);
}

EditorLayer::EditorLayer(int width, int height)
	:m_ViewPortWidth(width), m_ViewPortHeight(height)
{
	Agina::FramebufferSpecification spec;
	spec.Width = m_ViewPortWidth;
	spec.Height = m_ViewPortHeight;

	spec.Attachments = {
		 Agina::FramebufferTextureFormat::RGBA8,        // GL_COLOR_ATTACHMENT0
		Agina::FramebufferTextureFormat::RED_INTEGER,   // GL_COLOR_ATTACHMENT1 (Entity IDs)
		Agina::FramebufferTextureFormat::DEPTH24STENCIL8
	};

	m_Framebuffer = std::make_Ref<Framebuffer>(spec);
}

void EditorLayer::OnDetach() {}
