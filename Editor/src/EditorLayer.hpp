#pragma once
#include <Core/LayerStack/Layer.hpp>
#include <ECS/Scene.hpp>

class EditorLayer : public Agina::Layer
{
	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Agina::Event& e) override;
	void OnUpdate(float dt) override;
	void OnRender() override;
	void OnUIRender() override;

private:
	Agina::Scene m_Scene;
};
