#pragma once

#include <event/events.hpp>
#include <iostream>
#include <memory>

class Layer
{
public:
	virtual void onEvent(std::shared_ptr<Event> e) = 0;
};
