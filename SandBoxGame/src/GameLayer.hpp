#pragma once
#include <Core/LayerStack/Layer.hpp>
#include "Scene.hpp"

class GameLayer : public Agina::Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Agina::Event& e) override;
	void OnUpdate(float dt) override;
	void OnRender() override;
	void OnUIRender() override;

private:
	int m_ActiveSceneIndex = 0;
	std::vector<std::unique_ptr<Scene>> m_Scenes;
	entt::entity m_Camera;
};
