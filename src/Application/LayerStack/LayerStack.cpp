#include <Application/LayerStack/LayerStack.hpp>

void LayerStack::OnAttach()
{
	for (auto layer : m_Layers)
		layer->OnAttach();
}

void LayerStack::OnDetach()
{
	for (auto layer : m_Layers)
		layer->OnDetach();
}

void LayerStack::OnEvent(Event& e)
{
	for (auto layer : m_Layers)
		layer->OnEvent(e);
}

void LayerStack::OnUpdate(float dt)
{
	for (auto layer : m_Layers)
		layer->OnUpdate(dt);
}

void LayerStack::OnRender()
{
	for (auto layer : m_Layers)
		layer->OnRender();
}

LayerStack::~LayerStack()
{
	for (auto layer : m_Layers)
		delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.push_back(layer);
}

void LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end()) m_Layers.erase(it);
}

std::vector<Layer*>::iterator LayerStack::end() 
{
	return m_Layers.end();
}

std::vector<Layer*>::iterator LayerStack::begin()
{ 
	return m_Layers.begin();
}


