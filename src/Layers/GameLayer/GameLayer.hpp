#pragma once

#include <Application/LayerStack/Layer.hpp>

#include <Layers/GameLayer/Camera/Camera.hpp>
#include <Layers/GameLayer/Scene/SceneManager.hpp>

class GameLayer : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnEvent(Event& e) override;

private:
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<SceneManager> m_SceneManager;
};
