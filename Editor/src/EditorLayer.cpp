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

	auto camera = m_Scene.CreateEntity();
	auto& camereComp = camera.AddComponent<CameraComponent>();
	camera.AddComponent<TagComponent>("Camera");
	camera.AddComponent<Transform>(camereComp.Cam.GetRefPos());

	auto sponza = m_Scene.CreateEntity();
	sponza.AddComponent<TagComponent>("Sponza");
	sponza.AddComponent<ModelComponent>(AssetManager::LoadModel("sponza",
		assetPath + "/models/sponza/sponza.obj"));
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
