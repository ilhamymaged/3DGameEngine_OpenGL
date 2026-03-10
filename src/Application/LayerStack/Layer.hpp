#pragma once

#include <Application/Inputs/Events.hpp>
#include <iostream>
#include <memory>

class Renderer;
class Layer
{
public:
	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;
	virtual void OnEvent(Event& e) = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnRender(Renderer& r) = 0;
};
