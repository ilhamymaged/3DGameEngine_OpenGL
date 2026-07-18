#pragma once
#include <Core/Layer.hpp>
#include <ECS/Scene.hpp>
#include "SceneHierarchyPanel.hpp"
#include <Renderer/Framebuffer.hpp>

class EditorLayer : public Agina::Layer
{
public:
	EditorLayer(int width, int height);

	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Agina::Event& e) override;
	void OnUpdate(float dt) override;
	void OnRender() override;
	void OnUIRender() override;

private:
	Agina::Scene m_Scene;
	SceneHierarchyPanel m_Panel;

	int m_Width;
	int m_Height;
	std::shared_ptr<Framebuffer> m_Framebuffer;
};
