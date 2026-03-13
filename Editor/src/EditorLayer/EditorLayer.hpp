#pragma once
#include <Core/LayerStack/Layer.hpp>
#include <ECS/Entity.hpp>
#include <Scene/Scene.hpp>
#include <ECS/Components.hpp>
#include <Renderer/FrameBuffer.hpp>
#include "SceneState.hpp"
#include <glm/glm.hpp>

namespace Agina
{

	class EditorLayer : public Layer
	{
		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;
		void OnUpdate(float dt) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<Scene> m_EditorScene;
		Scene* m_ActiveScene;

		Entity m_SelectedEntity;
		std::unique_ptr<Framebuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize;
		SceneState m_SceneState = SceneState::Edit;
	};
}