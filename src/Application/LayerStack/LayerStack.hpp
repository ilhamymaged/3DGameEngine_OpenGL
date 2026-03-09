#pragma once
#include <Application/LayerStack/Layer.hpp>
#include <Application/Inputs/Events.hpp>
#include <vector>

class LayerStack : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Event& e) override;
	void OnUpdate(float dt) override;

	~LayerStack();

private:
	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	std::vector<Layer*>::iterator begin();
	std::vector<Layer*>::iterator end();

private:
	std::vector<Layer*> m_Layers;
};
