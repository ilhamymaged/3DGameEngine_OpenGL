#pragma once
#include <Core/Layer.hpp>
#include <Core/Events.hpp>
#include <vector>

namespace Agina
{
	class LayerStack
	{
	public:
		void OnAttach();
		void OnDetach();
		void OnEvent(Event& e);
		void OnUpdate(float dt);
		void OnRender();
		void OnUIRender();	

		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		inline std::vector<Layer*> GetLayers() { return m_Layers; }

	private:
		std::vector<Layer*>::iterator begin();
		std::vector<Layer*>::iterator end();

	private:
		std::vector<Layer*> m_Layers;
	};
}