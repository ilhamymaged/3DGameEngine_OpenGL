#pragma once
#include <Core/Layer.hpp>
#include <ECS/Scene.hpp>
#include "SceneHierarchyPanel.hpp"
#include <Renderer/Framebuffer.hpp>
#include <Agina.h>

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

	int m_ViewPortWidth;
	int m_ViewPortHeight;
	std::Ref<Framebuffer> m_Framebuffer;
};	
