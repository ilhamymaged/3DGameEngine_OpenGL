#include "EditorLayer.hpp"
#include "SceneHierarchy.hpp"
#include "Inspector.hpp"
#include <ImGui/ImGuiLayer.hpp>
#include <Renderer/Renderer.hpp>
#include <Core/Utility/LocU.hpp>
#include <Systems/Systems.hpp>
#include "ViewPort.hpp"
#include <Core/Inputs/Inputs.hpp>

namespace Agina
{
	void EditorLayer::OnAttach()
	{
		m_EditorScene = std::make_unique<Scene>();

		auto backpack = m_EditorScene->CreateEntity("BackPack");
		Model model("backpack/backpack.obj");
		Transform t(glm::vec3(0.0f, 2.0f, 0.0f));
		backpack.AddComponent<Transform>(t);
		backpack.AddComponent<MeshRenderer>(model);

		auto grid = m_EditorScene->CreateEntity("Grid");
		grid.AddComponent<GridComponent>();

		m_ViewportSize = { 800, 600 };
		m_FrameBuffer = std::make_unique<Framebuffer>((uint32_t)
			m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		auto camera = m_EditorScene->CreateEntity("Camera");
		camera.AddComponent<CameraComponent>
		(Camera(
			static_cast<int>(m_FrameBuffer->GetWidth()),
			static_cast<int>(m_FrameBuffer->GetHeight())
			));

		m_ActiveScene = m_EditorScene.get();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnEvent(Event& e)
	{
		if(m_SceneState == SceneState::Play)
			CameraSystem::onEvent(m_EditorScene->m_Registry, e);
	}
	void EditorLayer::OnUpdate(float dt)
	{
		if (m_SceneState == SceneState::Edit)
			CameraSystem::OnUpdate(m_ActiveScene->m_Registry, dt);
		else if (m_SceneState == SceneState::Play)
		{
			if (Input::Get().IsKeyPressed(GLFW_KEY_ESCAPE))
				Input::Get().UnLockMouse();
		}
	}

	void EditorLayer::OnRender()
	{
		m_FrameBuffer->Bind();

		Renderer::SetViewport(0, 0, m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight());
		Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderSystem::OnRender(m_ActiveScene->m_Registry);

		m_FrameBuffer->Unbind();
	}
	void EditorLayer::OnImGuiRender()
	{
        ImGuiLayer::EnableDocking();
		DrawViewPort(m_ActiveScene, m_EditorScene, m_FrameBuffer.get(), m_ViewportSize, m_SceneState);
		DrawSceneHierarchy(m_ActiveScene, m_SelectedEntity);
		DrawInspector(m_ViewportSize, m_SceneState, m_SelectedEntity, m_ActiveScene);
	}
}
