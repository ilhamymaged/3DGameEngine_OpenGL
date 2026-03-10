#pragma once

#include <Application/LayerStack/Layer.hpp>

#include <Layers/GameLayer/Camera/Camera.hpp>
#include <Layers/GameLayer/Scene/SceneManager.hpp>

class GameLayer : public Layer
{
public:
	GameLayer(int width, int height);

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnEvent(Event& e) override;
	void OnRender(Renderer& r) override;

	inline SceneManager& GetSceneManager() { return *m_SceneManager; }

private:
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<SceneManager> m_SceneManager;
};
