#pragma once

#include <Core/Inputs/Events.hpp>
#include <iostream>
#include <memory>

namespace Agina
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnEvent(Event& e) = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
	};
}
