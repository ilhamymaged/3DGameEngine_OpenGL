#pragma once
#include <Core/LayerStack/Layer.hpp>
#include <Core/Inputs/Events.hpp>
#include <vector>

namespace Agina
{
	class LayerStack : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;
		void OnUpdate(float dt) override;
		void OnRender() override;
		void OnImGuiRender() override;

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